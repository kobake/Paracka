class RndTable;

#ifndef _RndTable_
#define _RndTable_

class RndTable{
protected:
	int *list; //乱数テーブル
	int size;  //サイズ
	int index; //インデックス
	int last_value;
	int deleted_index;
public:
	//コンストラクタ・デストラクタ
	RndTable();
	~RndTable();
	//設定・取得
	void create(int _size);
	int getSize();
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

#endif
