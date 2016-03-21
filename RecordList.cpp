#include "include_com.h"
#include "RecordList.h"
#include <StringLib.h>
#include <algorithm>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �R���X�g���N�^�E�f�X�g���N�^
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
RecordList::RecordList()
{
	ans_flag=0;
	//
	turned=0;
	m_markingOnly = false;
}

RecordList::~RecordList()
{
	dispose();
}

void RecordList::dispose()
{
	for(int i = 0; i < (int)m_list.size(); i++){
		delete m_list[i];
	}
	m_list.clear();
	ans_flag=0;
//	turned=0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ���Ɖ� 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int RecordList::getValidCount()
{
	int cnt = 0;
	for(int i = 0; i < (int)m_list.size(); i++){
		if(isValidRecord(*m_list[i]))cnt++;
	}
	return cnt;
}

void RecordList::turn()
{
	turned=(1-turned);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �|�C���^���X�g
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void RecordList::_listDeleteAll()
{
	dispose();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �t�@�C�����o�� 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool RecordList::_read(FileStream *in, const mystring& filepath)
{
	// _listDeleteAll();
	//�S�e�L�X�g�ǂݍ��� �� textbuf
	long nbuf=in->getLength();
	std::vector<char> textbuf(nbuf+1);
	nbuf=in->read(&textbuf[0],nbuf);
	textbuf[nbuf]='\0';

	// �G���R�[�f�B���O�ϊ�: UTF-8 char[] -> wchar_t[]
	int wbuflen = ::MultiByteToWideChar(CP_UTF8, 0, &textbuf[0], nbuf, NULL, 0);
	std::vector<wchar_t> wbuf(wbuflen + 1);
	wbuflen = ::MultiByteToWideChar(CP_UTF8, 0, &textbuf[0], nbuf, &wbuf[0], wbuflen);
	wbuf[wbuflen] = 0;

	// \r�͎�菜��
	::wcsreplace(&wbuf[0], L"\r", L"");

	// �s���X�g�ɕϊ�
	std::vector<mystring> lines;
	{
		const wchar_t* p = &wbuf[0];
		const wchar_t* start = p;
		const wchar_t* end = p;
		while(1){
			if(*p == L'\n' || *p == L'\0'){
				const wchar_t* end = p;
				mystring line(start, p);
				lines.push_back(line);
				if(*p == L'\0')break;
				p++;
				start = p;
			}
			else{
				p++;
			}
		}
	}

	// �ǂݎ��
	bool prevMarked = false;
	for(int i = 0; i < (int)lines.size(); i++){
		mystring line = lines[i];
		// ��s
		if(line.length() == 0){
			m_list.push_back(new CommentRecord(filepath, line));
			continue;
		}
		// �}�[�L���O���o
		if(line == L"# ��"){
			prevMarked = true;
			continue;
		}
		// �R�����g���o
		if(line.startsWith(L"//") || line.startsWith(L"#")){
			m_list.push_back(new CommentRecord(filepath, line));
			continue;
		}
		// �e�L�X�g
		if(line.length() > 0){
			Record* lastRecord = NULL;
			if(m_list.size() > 0)lastRecord = m_list.back();

			if(lastRecord && lastRecord->isNormal() && lastRecord->m_a.length() == 0){
				lastRecord->m_a = line;
			}
			else{
				m_list.push_back(new NormalRecord(filepath, line, prevMarked));
			}
			prevMarked = false;
			continue;
		}
	}
	
	return true;
}

bool RecordList::loadFile(const std::vector<std::wstring>& paths)
{
	_listDeleteAll();
	for(int i = 0; i < (int)paths.size(); i++){
		FileStream in;
		if(!in.open(paths[i].c_str(), L"rt"))return false;
		bool ret = _read(&in, paths[i]);
		in.close();
		if(!ret)return false;
	}
	return true;
}

void RecordList::saveFile()
{
	std::map<std::wstring, FILE*> fps;
	for(int i = 0; i < (int)this->m_list.size(); i++){
		Record& record = *m_list[i];
		if(fps[record.filepath] == NULL){
			fps[record.filepath] = _wfopen(record.filepath.c_str(), L"wt");
		}
		record.addToFile(fps[record.filepath]);
	}
	std::for_each(fps.begin(), fps.end(), [](const std::pair<std::wstring, FILE*>& e){
		fclose(e.second);
	});
}
