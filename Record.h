#pragma once

#include <StringLib.h>
class RecordList;

struct Record{
	mystring q;
	mystring filepath;
	myvector<mystring> a;
	int kind;

	Record()
	{
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

