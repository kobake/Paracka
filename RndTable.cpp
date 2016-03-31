#include "include_com.h"
#include "RndTable.h"
#include "RecordList.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �R���X�g���N�^�E�f�X�g���N�^
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
RndTable::RndTable()
{
	index=-1;
	last_value=-1;
	deleted_index=-1;
	m_firstSize = 0;
}

RndTable::~RndTable()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//  �ݒ襎擾 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void RndTable::generateTable(const RecordList& list)
{
	//���X�g�쐬
	this->m_list.clear();
	for(int i = 0; i < list.getTotalCount(); i++){
		if(list.isValidRecord(list.getTotalAt(i))){
			this->m_list.push_back(i);
		}
	}
	m_firstSize = (int)m_list.size();

	//��������
	//_shuffle();  �� getNext �ż���ق����̂ł����ōs���K�v�͂Ȃ�
	//
	index=-1;
	last_value=-1;
	deleted_index=-1;
}

//value���o��Ώۂ��珜��
void RndTable::clear(int value)
{
	deleted_index=_delete(value);
}

void RndTable::clearAll()
{
	m_list.clear();
	index=-1;
	last_value=-1;
	deleted_index=-1;
}
bool RndTable::exists(int value)
{
	for(int i=0;i<(int)m_list.size();i++){
		if(m_list[i]==value)return true;
	}
	return false;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �g�p  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//rndtable[��萔]={1,3,2,9,4,8,2,...}
int RndTable::getNext()
{
	if(m_list.size() > 0){
		if(deleted_index>=0 && index>=deleted_index){
			index=index % getCurrentSize(); // index �͂��̂܂�
		}else{
			index=(index+1) % getCurrentSize();
		}
		deleted_index=-1;
		//
		if(index==0){
			_shuffle();
			if(m_list[0] == last_value && getCurrentSize() >= 2){ //���̒l(list[0])�ƑO�̒l(last_value)�����Ԃ邱�Ƃ����
				int tmp;
				int i=rand() % (getCurrentSize() - 1) + 1;
				m_swap(m_list[0], m_list[i], tmp);
			}
		}
		last_value = m_list[index];
		
		return m_list[index];
	}else{
		last_value=-1;
		return -1;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ����  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void RndTable::_shuffle()
{
	int i,a,b,t;
	if(getCurrentSize() >= 2){
		for(i=0;i<getCurrentSize()*10;i++){
			a=rand() % getCurrentSize(); //0(size-1)
			b=( a + 1 + rand()%(getCurrentSize()-1) ) % getCurrentSize(); //0(size-1)�̂��� a �ȊO
			m_swap(m_list[a],m_list[b],t);
		}
	}
}

int RndTable::_delete(int value)
{
	int i;
	for(i=0;i<getCurrentSize();i++){
		if(m_list[i]==value){
			m_list.erase(m_list.begin() + i);
			return i; //�폜�����l�̃C���f�b�N�X��Ԃ�
		}
	}
	return -1;
}
