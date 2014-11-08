#include "include_com.h"
#include "QesData.h"
#include "util/mywstring.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  �R���X�g���N�^�E�f�X�g���N�^  -- -- -- -- -- -- -- -- -- -- -- -- -- //
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
// -- -- -- -- -- -- -- -- -- -- -- -- ���Ɖ�  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
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
// -- -- -- -- -- -- -- -- -- -- -- -- -- �|�C���^���X�g -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
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
			//�P�ꃂ�[�h
			list[nlist-1].set_kind(0);
			list[nlist-1].put_q(p);
			ans_flag=1;
		}else{
			//�����߃��[�h
			list[nlist-1].set_kind(1);
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
						list[nlist-1].put_a(a);
						//��蕶�ւ̋󔒂ƶ���}��
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
// -- -- -- -- -- -- -- -- -- -- -- -- �t�@�C�����o��  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool QesData::read(FileStream *in)
{
	_listDeleteAll();
	//�S�e�L�X�g�ǂݍ��� �� textbuf
	long nbuf=in->getLength();
	char *textbuf=(char*)malloc(nbuf+1);
	nbuf=in->read(textbuf,nbuf);
	textbuf[nbuf]='\0';
	//�|�C���^���X�g�̍쐬
	char *p=textbuf;
	char *begin=p;
	int is_text=0;
	int is_comment=0;
	while(*p!='\0'){
		if(is_comment==0){
			//�R�����g���o
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
			//�g�[�N�����o
			if(*p==L'\\'){
				//���s�ϊ�
				if(*(p+1)=='\n'){		//�s���� \ ���������ꍇ�A���s
					*p++='\r';
					*p='\n';
				}else if(*(p+1)=='n'){	//�g�[�N������ \n ���������ꍇ�A���s
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
			//�R�����g�X�L�b�v
			if(*p=='\n'){
				is_comment=0;
			}
			*p='\0';
		}else if(is_comment==2){
			//�R�����g�X�L�b�v
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

