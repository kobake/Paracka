#pragma once

#include <StringLib.h>
class RecordList;

struct Record{
public:
	mystring			filepath;
	mystring			q;
	mystring			a;

public:
	Record()
	{
	}
	virtual ~Record()
	{
	}
	virtual bool isNormal() const	{ return false; }
	
	mystring getFileName() const;
};

class NormalRecord : public Record{
public:
	NormalRecord(const mystring& q, const mystring& filepath)
	{
		this->q = q;
		this->filepath = filepath;
	}
	virtual bool isNormal() const	{ return true; }
};

class CommentRecord : public Record{
public:
	CommentRecord(const mystring& text, const mystring& filepath)
	{
		this->q = text;
		this->filepath = filepath;
	}
};
