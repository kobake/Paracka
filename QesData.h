#pragma once

#include <StringLib.h>

#include "Record.h"

class QesData{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	QesData();
	virtual ~QesData();
	//���Ɖ�
	int getAllNum();
	mystring getQes(int index);
	mystring getFileName(int index);
	mystring getAns(int q_index,int a_index);
	int getAnsNum(int q_index){ return (int)list[q_index].a.size(); }
	int getQesKind(int q_index){ return list[q_index].kind; }
	void turn();
	//�|�C���^���X�g
	void _listDeleteAll();        //�|�C���^���X�g���ׂč폜
	void _listAdd(const wchar_t *p, const mystring& filepath); //�|�C���^���X�g�̒ǉ�
	//�t�@�C��
	bool loadFile(const std::vector<std::wstring>& paths);
	void dispose();
private:
	bool _read(FileStream *in, const mystring& filepath);
private:
	myvector<Record> list;
	int ans_flag;
	//���]�t���O
	int turned;
};


/*
	//�����e�[�u��
	int *rndtable; //�����e�[�u��
	int n_rndtable;
	int i_rndtable; //�����e�[�u���C���f�b�N�X
	int *cnttable; //�o��񐔋L�^�e�[�u��
	//�����e�[�u��
	int getRndIndex();
	void _shuffleRndTable();
	void _createCntTable();
	void _createRndTable();
	void _deleteRndIndex(int index);
	void clearQes(int index); //index���o��Ώۂ��珜��
	int getRestNum();
*/


/*
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//  �����e�[�u�� 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//rndtable[��萔]={1,3,2,9,4,8,2,...}
int QesData::getRndIndex()
{
	if(n_rndtable>0){
		int ret=rndtable[i_rndtable%n_rndtable];
		cnttable[ret]++;
		i_rndtable=(i_rndtable+1)%n_rndtable;
		if(i_rndtable==0){
			_shuffleRndTable();
			if(rndtable[0]==ret && n_rndtable>=2){ //���̖��(rndtable[0]�ƍ��̖��(ret)�����Ԃ邱�Ƃ����
				int tmp;
				int i=rand() % (n_rndtable-1) + 1;
				m_swap(rndtable[0],rndtable[i],tmp);
			}
		}
		return ret;
	}else{
		return -1;
	}
}

void QesData::_createCntTable()
{
	int i;
	if(nqes>0){
		cnttable=(int*)realloc(cnttable,sizeof(int)*nqes);
		for(i=0;i<nqes;i++){
			cnttable[i]=0;
		}
	}else{
		m_free(cnttable);
	}
}

void QesData::_shuffleRndTable()
{
	int i,a,b,t;
	if(n_rndtable>=2){
		for(i=0;i<n_rndtable*10;i++){
			a=rand() % n_rndtable; //0(n_rndtable-1)
			b=( a + 1 + rand()%(n_rndtable-1) ) %n_rndtable; //0(n_rndtable-1)�̂��� a �ȊO
			m_swap(rndtable[a],rndtable[b],t);
		}
	}
}

void QesData::_createRndTable()
{
	int i;
	n_rndtable=nqes;
	if(n_rndtable>0){
		rndtable=(int*)realloc(rndtable,sizeof(int)*n_rndtable);
		//������
		for(i=0;i<n_rndtable;i++){
			rndtable[i]=i;
		}
		//��������
		_shuffleRndTable();
	}else{
		m_free(rndtable);
	}
	i_rndtable=0;
}
void QesData::_deleteRndIndex(int index)
{
	int i;
	for(i=0;i<n_rndtable;i++){
		if(rndtable[i]==index){
			memmove(&rndtable[i],&rndtable[i+1],sizeof(int)*(n_rndtable-i-1));
			n_rndtable--;
			if(n_rndtable>0){
				rndtable=(int*)realloc(rndtable,sizeof(int)*n_rndtable);
				i_rndtable=(i_rndtable-1+n_rndtable) % n_rndtable;
			}else{
				m_free(rndtable);
				i_rndtable=0;
			}
			//�ꉞ break �����ɁA���ׂĂɂ��Č���
		}
	}
}

//index���o��Ώۂ��珜��
void QesData::clearQes(int index)
{
	if(index>=0 && index<nqes){
		_deleteRndIndex(index);
	}
}

int QesData::getRestNum()
{
	return n_rndtable;
}
*/
