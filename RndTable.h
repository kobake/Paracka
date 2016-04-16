#pragma once

#include <vector>
class RecordList;
class ClearStates;

class RndTable{
protected:
	std::vector<int>	m_list;
	int					m_firstSize;
	
	int index; //インデックス
	int last_value;
	int deleted_index;

	const RecordList*	m_recordlist;
public:
	//コンストラクタ・デストラクタ
	RndTable();
	~RndTable();
	//設定・取得
	void generateTable(const RecordList& list);
	int getCurrentSize() const	{ return (int)m_list.size(); }
	int getFirstSize() const	{ return m_firstSize; }
	void clear(int value); //valueを出題対象から除去
	void clearAll();
	bool exists(int value) const;
	//使用
	int getNext();
	//内部
	void _create(int _size);
	void _shuffle();
	int _delete(int value);

	// 回答状態
	ClearStates getClearStates(const RecordList& recordList) const;
	void applyClearStates(const RecordList& recordList, const ClearStates& states);

};
