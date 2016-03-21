#pragma once

#include <StringLib.h>
class RecordList;

struct Record{
public:
	mystring			filepath;
	mystring			q;
	myvector<mystring>	a;
	int					kind;

public:
	Record()
	{
		kind = 0;
	}
	void put_q(const wchar_t *_q, const mystring& _filepath)
	{
		q = _q;
		filepath = _filepath;
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

class NormalRecord : public Record{
public:
	NormalRecord(const mystring& q, const mystring& filepath)
	{
		this->put_q(q.c_str(), filepath);
		this->set_kind(0);
	}
};

class AnaumeRecord : public Record{
public:
	AnaumeRecord()
	{
		this->set_kind(1);
	}
};

