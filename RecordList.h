#pragma once

#include <StringLib.h>

#include "Record.h"

class RecordList{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RecordList();
	virtual ~RecordList();
	//���Ɖ�
	int getNormalCount();
	Record& getNormalAt(int index)
	{
		int normalIndex = 0;
		for(int i = 0; i < (int)m_list.size(); i++){
			if(m_list[i]->isNormal()){
				if(normalIndex == index){
					return *m_list[i];
				}
				normalIndex++;
			}
		}
		throw std::exception("getNormalAt: record not found");
	}

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

