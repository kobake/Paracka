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

	void toggleMarking()
	{
		if(m_marking == L""){
			m_marking = L"# ★";
		}
		else{
			m_marking = L"";
		}
	}

	mystring getMarkingPrefix() const
	{
		if(m_marking == L"")return L"";
		return L"★\r\n";
	}

	// ファイル
	void addToFile(FILE* fp) const
	{
		if(m_marking.length()){
			_writeAsUtf8(fp, L"%ls\n", m_marking.c_str());
		}
		_writeAsUtf8(fp, L"%ls\n", m_q.c_str());
		if(m_a.length()){
			_writeAsUtf8(fp, L"%ls\n", m_a.c_str());
		}
	}
	void _writeAsUtf8(FILE* fp, const wchar_t* str, ...) const
	{
		// 文字列フォーマット
		wchar_t buf[2048];
		va_list v;
		va_start(v, str);
		_vswprintf(buf, str, v);
		va_end(v);

		// UTF8変換
		std::string utf8 = unicode2utf8(buf);

		// 書き込み
		fwrite(utf8.c_str(), 1, utf8.length(), fp);
	}

	// 表示文字列
	mystring getQuestionText() const
	{
		return getMarkingPrefix() + m_q;
	}
	mystring getQuestionWithAnswerText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n%ls", this->m_q.c_str(), this->m_a.c_str());
		return getMarkingPrefix() + buf;
	}
	mystring getGoodText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n ++ ++ Good !! ++ ++", this->m_q.c_str());
		return getMarkingPrefix() + buf;
	}
	mystring getBadText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n ++ ++ Bad ... ++ ++", this->m_q.c_str());
		return getMarkingPrefix() + buf;
	}
};

class NormalRecord : public Record{
public:
	NormalRecord(const mystring& filepath, const mystring& q, bool marked)
	{
		this->filepath = filepath;
		this->m_q = q;
		if(marked){
			this->m_marking = L"# ★";
		}
	}
	virtual bool isNormal() const	{ return true; }
};

class CommentRecord : public Record{
public:
	CommentRecord(const mystring& filepath, const mystring& text)
	{
		this->filepath = filepath;
		this->m_q = text;
	}
};
