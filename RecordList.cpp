#include "include_com.h"
#include "RecordList.h"
#include <StringLib.h>


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// コンストラクタ・デストラクタ
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
// 問題と解答 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int RecordList::getNormalCount()
{
	int cnt = 0;
	for(int i = 0; i < (int)m_list.size(); i++){
		if(m_list[i]->isNormal())cnt++;
	}
	return cnt;
}

void RecordList::turn()
{
	turned=(1-turned);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ポインタリスト
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void RecordList::_listDeleteAll()
{
	dispose();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ファイル入出力 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool RecordList::_read(FileStream *in, const mystring& filepath)
{
	// _listDeleteAll();
	//全テキスト読み込み → textbuf
	long nbuf=in->getLength();
	std::vector<char> textbuf(nbuf+1);
	nbuf=in->read(&textbuf[0],nbuf);
	textbuf[nbuf]='\0';

	// エンコーディング変換: UTF-8 char[] -> wchar_t[]
	int wbuflen = ::MultiByteToWideChar(CP_UTF8, 0, &textbuf[0], nbuf, NULL, 0);
	std::vector<wchar_t> wbuf(wbuflen + 1);
	wbuflen = ::MultiByteToWideChar(CP_UTF8, 0, &textbuf[0], nbuf, &wbuf[0], wbuflen);
	wbuf[wbuflen] = 0;

	// \rは取り除く
	::wcsreplace(&wbuf[0], L"\r", L"");

	// 行リストに変換
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

	// 読み取り
	for(int i = 0; i < (int)lines.size(); i++){
		mystring line = lines[i];
		// コメント検出
		if(line.startsWith(L"//") || line.startsWith(L"#")){
			m_list.push_back(new CommentRecord(line, filepath));
			continue;
		}
		// テキスト
		if(line.length() > 0){
			Record* lastRecord = NULL;
			if(m_list.size() > 0)lastRecord = m_list.back();

			if(lastRecord && lastRecord->isNormal() && lastRecord->a.length() == 0){
				lastRecord->a = line;
			}
			else{
				m_list.push_back(new NormalRecord(line, filepath));
			}
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

