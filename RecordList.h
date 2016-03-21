#pragma once

#include <StringLib.h>

#include "Record.h"

class RecordList{
public:
	//コンストラクタ・デストラクタ
	RecordList();
	virtual ~RecordList();
	// レコード判定
	bool isValidRecord(const Record& record){
		if(!record.isNormal())return false;
		if(this->m_markingOnly && record.m_marking.length() == 0)return false;
		return true;
	}
	//問題と解答
	int getValidCount();
	Record& getValidAt(int index)
	{
		int normalIndex = 0;
		for(int i = 0; i < (int)m_list.size(); i++){
			if(isValidRecord(*m_list[i])){
				if(normalIndex == index){
					return *m_list[i];
				}
				normalIndex++;
			}
		}
		throw std::exception("getValidAt: record not found");
	}

	void turn();
	void toggleFilter(){
		m_markingOnly = !m_markingOnly;
	}
	//ポインタリスト
	void _listDeleteAll();        //ポインタリストすべて削除
	void _listAdd(const wchar_t *p, const mystring& filepath); //ポインタリストの追加
	//ファイル
	bool loadFile(const std::vector<std::wstring>& paths);
	void saveFile();
	void dispose();
private:
	bool _read(FileStream *in, const mystring& filepath);
private:
	myvector<Record*> m_list;
	int ans_flag;
	//反転フラグ
	int turned;
	// フィルタリングフラグ
	bool m_markingOnly;
};

