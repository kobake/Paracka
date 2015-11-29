#include "include_com.h"
#include "RndTable.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// コンストラクタ・デストラクタ
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
RndTable::RndTable()
{
	list=NULL;
	size=0;
	index=-1;
	last_value=-1;
	deleted_index=-1;
}

RndTable::~RndTable()
{
	m_free(list);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//  設定･取得 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void RndTable::create(int _size)
{
	//リスト作成
	_create(_size);
	//混ぜ混ぜ
	//_shuffle();  ※ getNext でｼｬｯﾌﾙされるのでここで行う必要はない
	//
	index=-1;
	last_value=-1;
	deleted_index=-1;
}

//valueを出題対象から除去
void RndTable::clear(int value)
{
	deleted_index=_delete(value);
}

int RndTable::getSize()
{
	return size;
}
void RndTable::clearAll()
{
	m_free(list);
	size=0;
	index=-1;
	last_value=-1;
	deleted_index=-1;
}
bool RndTable::exists(int value)
{
	for(int i=0;i<size;i++){
		if(list[i]==value)return true;
	}
	return false;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 使用  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//rndtable[問題数]={1,3,2,9,4,8,2,...}
int RndTable::getNext()
{
	if(size>0){
		if(deleted_index>=0 && index>=deleted_index){
			index=index % size; // index はそのまま
		}else{
			index=(index+1)%size;
		}
		deleted_index=-1;
		//
		if(index==0){
			_shuffle();
			if(list[0]==last_value && size>=2){ //今の値(list[0])と前の値(last_value)がかぶることを回避
				int tmp;
				int i=rand() % (size-1) + 1;
				m_swap(list[0],list[i],tmp);
			}
		}
		last_value=list[index];
		//###デバッグ情報
		if(0){
			debugprintf(L"rnd: ");
			for(int i=0;i<size;i++){
				if(i==index){
					debugprintf(L"[%d] ",list[i]);
				}else{
					debugprintf(L"%d ",list[i]);
				}
			}
			debugprintf(L"\n");
		}
		return list[index];
	}else{
		last_value=-1;
		return -1;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 内部  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void RndTable::_create(int _size)
{
	int i;
	size=_size;
	if(size>0){
		list=(int*)realloc(list,sizeof(int)*size);
		//初期化
		for(i=0;i<size;i++){
			list[i]=i;
		}
	}else{
		m_free(list);
	}
}
void RndTable::_shuffle()
{
	int i,a,b,t;
	if(size>=2){
		for(i=0;i<size*10;i++){
			a=rand() % size; //0(size-1)
			b=( a + 1 + rand()%(size-1) ) % size; //0(size-1)のうち a 以外
			m_swap(list[a],list[b],t);
		}
	}
}

int RndTable::_delete(int value)
{
	int i;
	for(i=0;i<size;i++){
		if(list[i]==value){
			memmove(&list[i],&list[i+1],sizeof(int)*(size-i-1));
			size--;
			if(size>0){
				list=(int*)realloc(list,sizeof(int)*size);
			}else{
				m_free(list);
			}
			return i; //削除した値のインデックスを返す
		}
	}
	return -1;
}
