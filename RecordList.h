#pragma once

#include <StringLib.h>
#include "Record.h"
class ClearStates;

class RecordList{
public:
	//コンストラクタ・デストラクタ
	RecordList();
	virtual ~RecordList();
	// レコード判定
	bool isValidRecord(const Record& record) const{
		if(!record.isNormal())return false;
		if(m_filterLevel == -2){
			return record.m_markingLevel == 0; // 簡単のみ
		}
		else if(m_filterLevel == -1){
			return record.m_markingLevel == 1; // ★のみ
		}
		if(record.m_markingLevel < this->m_filterLevel)return false;
		return true;
	}
	//問題と解答
	int getTotalCount() const{
		return (int)m_list.size();
	}
	const Record& getTotalAt(int index) const{
		return *m_list[index];
	}

	void toggleFilter(){
		// -2:簡単のみ
		// -1:★のみ
		// 0:すべて
		// 1:★以上
		// 2:★★以上

		// -2,-1,0,1,2
		m_filterLevel++;
		if(m_filterLevel > 2)m_filterLevel = -2;
	}
	int getFilterLevel() const{
		return m_filterLevel;
	}
	mystring getFilterLevelString() const{
		switch(m_filterLevel){
		case -2: return L"簡単のみ";
		case -1: return L"★のみ";
		case 0: return L"全表示";
		case 1: return L"★以上";
		case 2: return L"★★以上";
		default: return L"？";
		}
	}
	//ポインタリスト
	void _listDeleteAll();        //ポインタリストすべて削除
	void _listAdd(const wchar_t *p, const mystring& filepath); //ポインタリストの追加
	//ファイル
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
	// フィルタリングレベル
	int m_filterLevel; // 0:全表示 1:★と★★を表示 2:★★のみ表示
};

