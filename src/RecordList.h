#pragma once

#include <StringLib.h>
#include "Record.h"
class ClearStates;

class RecordList{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	RecordList();
	virtual ~RecordList();
	// ���R�[�h����
	bool isValidRecord(const Record& record) const{
		if(!record.isNormal())return false;
		if(m_filterLevel == -2){
			return record.m_markingLevel == 0; // �ȒP�̂�
		}
		else if(m_filterLevel == -1){
			return record.m_markingLevel == 1; // ���̂�
		}
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

	void toggleFilter(){
		// -2:�ȒP�̂�
		// -1:���̂�
		// 0:���ׂ�
		// 1:���ȏ�
		// 2:�����ȏ�

		// -2,-1,0,1,2
		m_filterLevel++;
		if(m_filterLevel > 2)m_filterLevel = -2;
	}
	int getFilterLevel() const{
		return m_filterLevel;
	}
	mystring getFilterLevelString() const{
		switch(m_filterLevel){
		case -2: return L"�ȒP�̂�";
		case -1: return L"���̂�";
		case 0: return L"�S�\��";
		case 1: return L"���ȏ�";
		case 2: return L"�����ȏ�";
		default: return L"�H";
		}
	}
	//�|�C���^���X�g
	void _listDeleteAll();        //�|�C���^���X�g���ׂč폜
	void _listAdd(const wchar_t *p, const mystring& filepath); //�|�C���^���X�g�̒ǉ�
	//�t�@�C��
	bool loadFile(const std::vector<std::wstring>& paths);
	void saveFile();
	void dispose();
	mystring getFileMode() const	{ return m_filemode; }
private:
	bool _read(FileStream *in, const mystring& filepath);
private:
	mystring			m_filemode;
	myvector<Record*>	m_list;
	int ans_flag;
	// �t�B���^�����O���x��
	int m_filterLevel; // 0:�S�\�� 1:���Ɓ�����\�� 2:�����̂ݕ\��
};

