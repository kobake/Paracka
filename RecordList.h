#pragma once

#include <StringLib.h>

#include "Record.h"

class RecordList{
public:
	//コンストラクタ・デストラクタ
	RecordList();
	virtual ~RecordList();
	//問題と解答
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
	//ポインタリスト
	void _listDeleteAll();        //ポインタリストすべて削除
	void _listAdd(const wchar_t *p, const mystring& filepath); //ポインタリストの追加
	//ファイル
	bool loadFile(const std::vector<std::wstring>& paths);
	void dispose();
private:
	bool _read(FileStream *in, const mystring& filepath);
private:
	myvector<Record*> m_list;
	int ans_flag;
	//反転フラグ
	int turned;
};

