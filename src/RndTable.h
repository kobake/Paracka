#pragma once

#include <vector>
class RecordList;
class ClearStates;

class RndTable{
protected:
	std::vector<int>	m_list;
	int					m_firstSize;
	
	int index; //�C���f�b�N�X
	int last_value;
	int deleted_index;

	const RecordList*	m_recordlist;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RndTable();
	~RndTable();
	//�ݒ�E�擾
	void generateTable(const RecordList& list);
	int getCurrentSize() const	{ return (int)m_list.size(); }
	int getFirstSize() const	{ return m_firstSize; }
	void clear(int value); //value���o��Ώۂ��珜��
	void clearAll();
	bool exists(int value) const;
	//�g�p
	int getNext();
	//����
	void _create(int _size);
	void _shuffle();
	int _delete(int value);

	// �񓚏��
	ClearStates getClearStates(const RecordList& recordList) const;
	void applyClearStates(const RecordList& recordList, const ClearStates& states);

};
