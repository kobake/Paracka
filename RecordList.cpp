#include "include_com.h"
#include "RecordList.h"
#include <StringLib.h>


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �R���X�g���N�^�E�f�X�g���N�^
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
RecordList::RecordList()
{
	ans_flag=0;
	//
	turned=0;
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

int RecordList::getAllNum()
{
	if(ans_flag==0){
		return (int)m_list.size();
	}else{
		return (int)m_list.size() - 1;
	}
}

mystring RecordList::getQes(int index)
{
	if(index>=0 && index<getAllNum()){
		if(turned==0){
			return m_list[index]->q;
		}else{
			return m_list[index]->a[0];
		}
	}else{
		return NULL;
	}
}
mystring RecordList::getFileName(int index)
{
	if(index>=0 && index<getAllNum()){
		return CFilePath(m_list[index]->filepath).GetTitle();
	}else{
		return L"?";
	}
}

mystring RecordList::getAns(int q_index,int a_index)
{
	if(q_index>=0 && q_index<getAllNum()){
		if(turned==0){
			return m_list[q_index]->a[a_index];
		}else{
			return m_list[q_index]->q;
		}
	}else{
		return NULL;
	}
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

void RecordList::_listAdd(const wchar_t *p, const mystring& filepath)
{
	if(ans_flag==0){
		if(p[0]!='@'){
			//�P�ꃂ�[�h
			m_list.push_back(new NormalRecord(p, filepath));
			ans_flag=1;
		}else{
			//�����߃��[�h
			m_list.push_back(new AnaumeRecord());
			Record& qa = *m_list.back();
			//
			p++;
			//��� p �̒�����𓚁u(x)�v�𔲂��o��
			wchar_t _q[1024],*q=_q,a[256];
			const wchar_t *a_begin,*a_end; int a_len;
			int is_a=0;
			while(*p!='\0' && (q-_q)<1000){
				if(!is_a){
					if(*p==L'('){
						//�𓚊J�n�ʒu
						is_a=1;
						a_begin=p+1;
					}
					*q++=*p;
				}else{
					if(*p==L')'){
						//�𓚔͈͂̌���
						is_a=0;
						a_end=p;
						a_len=a_end-a_begin;
						if(a_len>255)a_len=255;
						//�𓚂̃R�s�[
						wcsncpy(a,a_begin,a_len);
						a[a_len]=L'\0';
						//
						qa.put_a(a);
						//��蕶�ւ̋󔒂ƶ���}��
						for(int s=0;s<4;s++)*q++=L' ';
						*q++=*p;
					}
				}
				p++;
			}
			*q=L'\0';
			qa.put_q(_q, filepath);
		}
	}
	else{
		m_list.back()->put_a(p);
		ans_flag=0;
	}
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
	for(int i = 0; i < (int)lines.size(); i++){
		mystring line = lines[i];
		// �R�����g���o
		if(line.startsWith(L"//") || line.startsWith(L"#")){
			continue;
		}
		// �e�L�X�g
		if(line.length() > 0){
			_listAdd(line.c_str(), filepath);
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

