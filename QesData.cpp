#include "include_com.h"
#include "QesData.h"
#include <StringLib.h>


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �R���X�g���N�^�E�f�X�g���N�^
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
QesData::QesData()
{
	list.clear();
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
	list.clear();
	ans_flag=0;
//	turned=0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ���Ɖ� 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int QesData::getAllNum()
{
	if(ans_flag==0){
		return (int)list.size();
	}else{
		return (int)list.size() - 1;
	}
}

mystring QesData::getQes(int index)
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
mystring QesData::getFileName(int index)
{
	if(index>=0 && index<getAllNum()){
		return CFilePath(list[index].filepath).GetTitle();
	}else{
		return L"?";
	}
}

mystring QesData::getAns(int q_index,int a_index)
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



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �|�C���^���X�g
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void QesData::_listDeleteAll()
{
	dispose();
}

void QesData::_listAdd(const wchar_t *p, const mystring& filepath)
{
	if(ans_flag==0){
		list.push_back(Record());
		Record& qa = list.back();


		if(p[0]!='@'){
			//�P�ꃂ�[�h
			qa.set_kind(0);
			qa.put_q(p, filepath);
			ans_flag=1;
		}else{
			//�����߃��[�h
			qa.set_kind(1);
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
	}else{
		list.back().put_a(p);
		ans_flag=0;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �t�@�C�����o�� 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool QesData::_read(FileStream *in, const mystring& filepath)
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
	
	//�|�C���^���X�g�̍쐬
	wchar_t *p=&wbuf[0];
	wchar_t *begin=p;
	int is_text=0;
	int is_comment=0;
	while(*p!='\0'){
		if(is_comment==0){
			//�R�����g���o (//�܂���#)
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
			//�g�[�N�����o
			if(*p==L'\\'){
				//���s�ϊ�
				if(*(p+1)==L'\n'){		//�s���� \ ���������ꍇ�A���s
					*p++=L'\r';
					*p=L'\n';
				}else if(*(p+1)=='n'){	//�g�[�N������ \n ���������ꍇ�A���s
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
			//�R�����g�X�L�b�v
			if(*p==L'\n'){
				is_comment=0;
			}
			*p=L'\0';
		}else if(is_comment==2){
			//�R�����g�X�L�b�v
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

bool QesData::loadFile(const std::vector<std::wstring>& paths)
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

