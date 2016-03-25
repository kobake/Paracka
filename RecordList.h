#pragma once

#include <StringLib.h>

#include "Record.h"

class RecordList{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RecordList();
	virtual ~RecordList();
	// ���R�[�h����
	bool isValidRecord(const Record& record) const{
		if(!record.isNormal())return false;
		if(record.m_markingLevel < this->m_filterLevel)return false;
		return true;
	}
	//���Ɖ�
	int getTotalCount() const{
		return (int)m_list.size();
	}
	const Record& getTotalAt(int index) const{
		return *m_list[index];
	}

	void turn();
	void toggleFilter(){
		m_filterLevel = (m_filterLevel + 1) % 3; // 0,1,2
	}
	int getFilterLevel() const{
		return m_filterLevel;
	}
	//�|�C���^���X�g
	void _listDeleteAll();        //�|�C���^���X�g���ׂč폜
	void _listAdd(const wchar_t *p, const mystring& filepath); //�|�C���^���X�g�̒ǉ�
	//�t�@�C��
	bool loadFile(const std::vector<std::wstring>& paths);
	void saveFile();
	void dispose();
private:
	bool _read(FileStream *in, const mystring& filepath);
private:
	myvector<Record*> m_list;
	int ans_flag;
	//���]�t���O
	int turned;
	// �t�B���^�����O���x��
	int m_filterLevel; // 0:�S�\�� 1:���Ɓ�����\�� 2:�����̂ݕ\��
};

