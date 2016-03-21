#pragma once

#include <StringLib.h>
class RecordList;

struct Record{
public:
	mystring			filepath;
	mystring			m_marking;
	mystring			m_q;
	mystring			m_a;

public:
	Record()
	{
	}
	virtual ~Record()
	{
	}
	virtual bool isNormal() const	{ return false; }
	
	mystring getFileName() const;

	// •\Ž¦•¶Žš—ñ
	mystring getQuestionText() const
	{
		return m_q;
	}
	mystring getQuestionWithAnswerText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n%ls", this->m_q.c_str(), this->m_a.c_str());
		return buf;
	}
	mystring getGoodText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n ++ ++ Good !! ++ ++", this->m_q.c_str());
		return buf;
	}
	mystring getBadText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n ++ ++ Bad ... ++ ++", this->m_q.c_str());
		return buf;
	}
};

class NormalRecord : public Record{
public:
	NormalRecord(const mystring& q, const mystring& filepath)
	{
		this->m_q = q;
		this->filepath = filepath;
	}
	virtual bool isNormal() const	{ return true; }
};

class CommentRecord : public Record{
public:
	CommentRecord(const mystring& text, const mystring& filepath)
	{
		this->m_q = text;
		this->filepath = filepath;
	}
};
