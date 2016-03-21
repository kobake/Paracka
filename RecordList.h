#pragma once

#include <StringLib.h>

#include "Record.h"

class RecordList{
public:
	//コンストラクタ・デストラクタ
	RecordList();
	virtual ~RecordList();
	//問題と解答
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
};

