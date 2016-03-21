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
// ポインタリスト
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void RecordList::_listDeleteAll()
{
	dispose();
}

void RecordList::_listAdd(const wchar_t *p, const mystring& filepath)
{
	if(ans_flag==0){
		if(p[0]!='@'){
			//単語モード
			m_list.push_back(new NormalRecord(p, filepath));
			ans_flag=1;
		}else{
			//穴埋めモード
			m_list.push_back(new AnaumeRecord());
			Record& qa = *m_list.back();
			//
			p++;
			//問題 p の中から解答「(x)」を抜き出す
			wchar_t _q[1024],*q=_q,a[256];
			const wchar_t *a_begin,*a_end; int a_len;
			int is_a=0;
			while(*p!='\0' && (q-_q)<1000){
				if(!is_a){
					if(*p==L'('){
						//解答開始位置
						is_a=1;
						a_begin=p+1;
					}
					*q++=*p;
				}else{
					if(*p==L')'){
						//解答範囲の決定
						is_a=0;
						a_end=p;
						a_len=a_end-a_begin;
						if(a_len>255)a_len=255;
						//解答のコピー
						wcsncpy(a,a_begin,a_len);
						a[a_len]=L'\0';
						//
						qa.put_a(a);
						//問題文への空白とｶｯｺ挿入
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
	
	//ポインタリストの作成
	wchar_t *p=&wbuf[0];
	wchar_t *begin=p;
	int is_text=0;
	int is_comment=0;
	while(*p!='\0'){
		if(is_comment==0){
			//コメント検出 (//または#)
			if(*p==L'/'){
				if(*(p+1)==L'/'){
					is_comment=1;
					is_text=0;
					continue;
				}else if(*(p+1)==L'*'){
					is_comment=2;
					is_text=0;
					continue;
				}
			}
			else if(*p == L'#'){
				is_comment = 1;
				is_text = 0;
				continue;
			}
			//トークン検出
			if(*p==L'\\'){
				//改行変換
				if(*(p+1)==L'\n'){		//行末に \ があった場合、改行
					*p++=L'\r';
					*p=L'\n';
				}else if(*(p+1)=='n'){	//トークン内に \n があった場合、改行
					*p++=L'\r';
					*p=L'\n';
				}else{
					goto token;
				}
			}else if(*p==L'\t' || *p==L'\n'){
				*p=L'\0';
				if(is_text==1){
					_listAdd(begin, filepath);
					is_text=0;
				}
			}else{
token:;
				if(is_text==0){
					begin=p;
					is_text=1;
				}
			}
		}else if(is_comment==1){
			//コメントスキップ
			if(*p==L'\n'){
				is_comment=0;
			}
			*p=L'\0';
		}else if(is_comment==2){
			//コメントスキップ
			if(*p==L'*' && *(p+1)==L'/'){
				*(p+1)=L'\0';
				is_comment=0;
			}
			*p=L'\0';
		}
		p++;
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

