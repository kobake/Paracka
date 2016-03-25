#pragma once

#include <vector>
class RecordList;

class RndTable{
protected:
	std::vector<int>	m_list;
	int					m_firstSize;
	
	int index; //インデックス
	int last_value;
	int deleted_index;
public:
	//コンストラクタ・デストラクタ
	RndTable();
	~RndTable();
	//設定・取得
	void create(const RecordList& list);
	int getCurrentSize() const	{ return (int)m_list.size(); }
	int getFirstSize() const	{ return m_firstSize; }
	void clear(int value); //valueを出題対象から除去
	void clearAll();
	bool exists(int value);
	//使用
	int getNext();
	//内部
	void _create(int _size);
	void _shuffle();
	int _delete(int value);
};
