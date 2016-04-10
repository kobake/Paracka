#pragma once

#include <StringLib.h>
class RecordList;

struct Record{
public:
	mystring			filepath;
	int					m_markingLevel;
	mystring			m_q;
	mystring			m_a;

public:
	Record()
	{
		m_markingLevel = 0;
	}
	virtual ~Record()
	{
	}
	virtual bool isNormal() const	{ return false; }
	
	mystring getFileName() const;

	void toggleMarking()
	{
		m_markingLevel = (m_markingLevel + 1) % 3; //0,1,2
	}
	mystring getMarkingString() const
	{
		mystring ret = L"";
		for(int i = 0; i < m_markingLevel; i++){
			ret += L"★";
		}
		return ret;
	}
	mystring getMarkingPrefix() const
	{
		mystring ret = getMarkingString();
		if(ret == L"")return L"";
		else return ret + L"\r\n";
	}

	// ファイル
	void addToFile(FILE* fp) const
	{
		mystring marking = getMarkingString();
		if(marking.length()){
			_writeAsUtf8(fp, L"# %ls\n", marking.c_str());
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

	
};



class NormalRecord : public Record{
public:
	NormalRecord(const mystring& filepath, const mystring& q, int markingLevel)
	{
		this->filepath = filepath;
		this->m_q = q;
		this->m_markingLevel = markingLevel;
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
