#pragma once

#include <StringLib.h>

#include "Record.h"

class RecordList{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RecordList();
	virtual ~RecordList();
	//���Ɖ�
	int getAllNum();
	Record& getRecord(int index)
	{
		return *m_list[index];
	}

	mystring getQes(int index);
	mystring getFileName(int index);
	mystring getAns(int q_index,int a_index);
	int getAnsNum(int q_index){ return (int)m_list[q_index]->a.size(); }
	//int getQesKind(int q_index){ return m_list[q_index]->kind; }
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
	myvector<Record*> m_list;
	int ans_flag;
	//���]�t���O
	int turned;
};

