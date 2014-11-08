class RndTable;

#ifndef _RndTable_
#define _RndTable_

class RndTable{
protected:
	int *list; //�����e�[�u��
	int size;  //�T�C�Y
	int index; //�C���f�b�N�X
	int last_value;
	int deleted_index;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RndTable();
	~RndTable();
	//�ݒ�E�擾
	void create(int _size);
	int getSize();
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

#endif
