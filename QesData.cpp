#include "include_com.h"
#include "QesData.h"
#include <StringLib.h>


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
QesData::QesData()
{
	list=NULL;
	nlist=0;
	ans_flag=0;
	//
	turned=0;
}

QesData::~QesData()
{
	dispose();
}

void QesData::dispose()
{
	for(int i=0;i<nlist;i++)list[i].dispose();
	m_free(list);
	nlist=0;
	ans_flag=0;
//	turned=0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 問題と解答  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int QesData::getAllNum()
{
	if(ans_flag==0){
		return nlist;
	}else{
		return nlist-1;
	}
}

const wchar_t *QesData::getQes(int index)
{
	if(index>=0 && index<getAllNum()){
		if(turned==0){
			return list[index].q;
		}else{
			return list[index].a[0];
		}
	}else{
		return NULL;
	}
}

const wchar_t *QesData::getAns(int q_index,int a_index)
{
	if(q_index>=0 && q_index<getAllNum()){
		if(turned==0){
			return list[q_index].a[a_index];
		}else{
			return list[q_index].q;
		}
	}else{
		return NULL;
	}
}

void QesData::turn()
{
	turned=(1-turned);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- ポインタリスト -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void QesData::_listDeleteAll()
{
	dispose();
}

void QesData::_listAdd(const wchar_t *p)
{
	if(ans_flag==0){
		nlist++;
		if(nlist % 256==1){
			list=(QA*)realloc(list,sizeof(QA)*(nlist+255));
		}
		list[nlist-1].zero();
		if(p[0]!='@'){
			//単語モード
			list[nlist-1].set_kind(0);
			list[nlist-1].put_q(p);
			ans_flag=1;
		}else{
			//穴埋めモード
			list[nlist-1].set_kind(1);
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
						list[nlist-1].put_a(a);
						//問題文への空白とｶｯｺ挿入
						for(int s=0;s<4;s++)*q++=L' ';
						*q++=*p;
					}
				}
				p++;
			}
			*q=L'\0';
			list[nlist-1].put_q(_q);
		}
	}else{
		list[nlist-1].put_a(p);
		ans_flag=0;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- ファイル入出力  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool QesData::read(FileStream *in)
{
	_listDeleteAll();
	//全テキスト読み込み → textbuf
	long nbuf=in->getLength();
	char *textbuf=(char*)malloc(nbuf+1);
	nbuf=in->read(textbuf,nbuf);
	textbuf[nbuf]='\0';
	//ポインタリストの作成
	char *p=textbuf;
	char *begin=p;
	int is_text=0;
	int is_comment=0;
	while(*p!='\0'){
		if(is_comment==0){
			//コメント検出 (//または#)
			if(*p=='/'){
				if(*(p+1)=='/'){
					is_comment=1;
					is_text=0;
					continue;
				}else if(*(p+1)=='*'){
					is_comment=2;
					is_text=0;
					continue;
				}
			}
			else if(*p == '#'){
				is_comment = 1;
				is_text = 0;
				continue;
			}
			//トークン検出
			if(*p==L'\\'){
				//改行変換
				if(*(p+1)=='\n'){		//行末に \ があった場合、改行
					*p++='\r';
					*p='\n';
				}else if(*(p+1)=='n'){	//トークン内に \n があった場合、改行
					*p++='\r';
					*p='\n';
				}else{
					goto token;
				}
			}else if(*p=='\t' || *p=='\n'){
				*p='\0';
				if(is_text==1){
					_listAdd(tmp_mbstowcs(begin));
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
			if(*p=='\n'){
				is_comment=0;
			}
			*p='\0';
		}else if(is_comment==2){
			//コメントスキップ
			if(*p=='*' && *(p+1)=='/'){
				*(p+1)='\0';
				is_comment=0;
			}
			*p='\0';
		}
		p++;
	}
	if(nlist % 2 == 1)_listAdd(L"");
//	free(this->textbuf); this->textbuf=textbuf;
	free(textbuf);
	return true;
}

bool QesData::loadFile(const wchar_t* fpath)
{
	_listDeleteAll();
	FileStream in;
	if(in.open(fpath,L"rt")){
		bool ret=read(&in);
		in.close();
		return ret;
	}else{
		return false;
	}
}

