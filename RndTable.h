#pragma once

#include <vector>
class RecordList;

class RndTable{
protected:
	std::vector<int>	m_list;
	int					m_firstSize;
	
	int index; //�C���f�b�N�X
	int last_value;
	int deleted_index;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RndTable();
	~RndTable();
	//�ݒ�E�擾
	void create(const RecordList& list);
	int getCurrentSize() const	{ return (int)m_list.size(); }
	int getFirstSize() const	{ return m_firstSize; }
	void clear(int value); //value���o��Ώۂ��珜��
	void clearAll();
	bool exists(int value);
	//�g�p
	int getNext();
	//����
	void _create(int _size);
	void _shuffle();
	int _delete(int value);
};
