#pragma once

#include <StringLib.h>
class QesData;

class QandA{
public:
private:
	mystring	m_q;
	mystring	m_a;
};

struct QA{
	mystring q;
	myvector<mystring> a;
	//wchar_t **a;
	//int na;
	int kind;
	QA(){
		kind = 0;
	}

	void dispose()
	{
		q = L"";
		a.clear();
	}
	void zero()
	{
		q = L"";
		a.clear();
		kind=0;
	}
	void put_q(const wchar_t *_q)
	{
		q = _q;
	}
	void put_a(const wchar_t *_a)
	{
		a.push_back(_a);
	}
	void set_kind(int k)
	{
		kind=k;
	}
};

class QesData{
public:
	//コンストラクタ・デストラクタ
	QesData();
	virtual ~QesData();
	//問題と解答
	int getAllNum();
	mystring getQes(int index);
	mystring getAns(int q_index,int a_index);
	int getAnsNum(int q_index){ return (int)list[q_index].a.size(); }
	int getQesKind(int q_index){ return list[q_index].kind; }
	void turn();
	//ポインタリスト
	void _listDeleteAll();        //ポインタリストすべて削除
	void _listAdd(const wchar_t *p); //ポインタリストの追加
	//ファイル
	bool loadFile(const std::vector<std::wstring>& paths);
	void dispose();
private:
	bool _read(FileStream *in);
private:
	myvector<QA> list;
	int ans_flag;
	//反転フラグ
	int turned;
};


/*
	//乱数テーブル
	int *rndtable; //乱数テーブル
	int n_rndtable;
	int i_rndtable; //乱数テーブルインデックス
	int *cnttable; //出問回数記録テーブル
	//乱数テーブル
	int getRndIndex();
	void _shuffleRndTable();
	void _createCntTable();
	void _createRndTable();
	void _deleteRndIndex(int index);
	void clearQes(int index); //indexを出題対象から除去
	int getRestNum();
*/


/*
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//  乱数テーブル 
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//rndtable[問題数]={1,3,2,9,4,8,2,...}
int QesData::getRndIndex()
{
	if(n_rndtable>0){
		int ret=rndtable[i_rndtable%n_rndtable];
		cnttable[ret]++;
		i_rndtable=(i_rndtable+1)%n_rndtable;
		if(i_rndtable==0){
			_shuffleRndTable();
			if(rndtable[0]==ret && n_rndtable>=2){ //次の問題(rndtable[0]と今の問題(ret)がかぶることを回避
				int tmp;
				int i=rand() % (n_rndtable-1) + 1;
				m_swap(rndtable[0],rndtable[i],tmp);
			}
		}
		return ret;
	}else{
		return -1;
	}
}

void QesData::_createCntTable()
{
	int i;
	if(nqes>0){
		cnttable=(int*)realloc(cnttable,sizeof(int)*nqes);
		for(i=0;i<nqes;i++){
			cnttable[i]=0;
		}
	}else{
		m_free(cnttable);
	}
}

void QesData::_shuffleRndTable()
{
	int i,a,b,t;
	if(n_rndtable>=2){
		for(i=0;i<n_rndtable*10;i++){
			a=rand() % n_rndtable; //0(n_rndtable-1)
			b=( a + 1 + rand()%(n_rndtable-1) ) %n_rndtable; //0(n_rndtable-1)のうち a 以外
			m_swap(rndtable[a],rndtable[b],t);
		}
	}
}

void QesData::_createRndTable()
{
	int i;
	n_rndtable=nqes;
	if(n_rndtable>0){
		rndtable=(int*)realloc(rndtable,sizeof(int)*n_rndtable);
		//初期化
		for(i=0;i<n_rndtable;i++){
			rndtable[i]=i;
		}
		//混ぜ混ぜ
		_shuffleRndTable();
	}else{
		m_free(rndtable);
	}
	i_rndtable=0;
}
void QesData::_deleteRndIndex(int index)
{
	int i;
	for(i=0;i<n_rndtable;i++){
		if(rndtable[i]==index){
			memmove(&rndtable[i],&rndtable[i+1],sizeof(int)*(n_rndtable-i-1));
			n_rndtable--;
			if(n_rndtable>0){
				rndtable=(int*)realloc(rndtable,sizeof(int)*n_rndtable);
				i_rndtable=(i_rndtable-1+n_rndtable) % n_rndtable;
			}else{
				m_free(rndtable);
				i_rndtable=0;
			}
			//一応 break せずに、すべてについて検索
		}
	}
}

//indexを出題対象から除去
void QesData::clearQes(int index)
{
	if(index>=0 && index<nqes){
		_deleteRndIndex(index);
	}
}

int QesData::getRestNum()
{
	return n_rndtable;
}
*/
