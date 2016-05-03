#pragma once

#include <StringLib.h>
#include "Record.h"

// ���[�e�B���e�B
mystring trimPartMark(mystring text);

class RecordVisitor{
private:
	const Record* m_record;
	bool m_turned;
public:
	RecordVisitor(const Record& record, bool turned)
	{
		m_record = &record;
		m_turned = turned;
	}
public:
	// ���]�Ή�
	mystring getQ() const
	{
		return !m_turned ? m_record->m_q : m_record->m_a;
	}
	mystring getA() const
	{
		return !m_turned ? m_record->m_a : m_record->m_q;
	}

	// �`�F�b�N
	bool checkAnswer(mystring ans) const;
	
	// �\��������
	mystring getQuestionText() const
	{
		return m_record->getMarkingPrefix() + getQ();
	}
	mystring getQuestionWithAnswerText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n%ls", getQ().c_str(), getA().c_str());
		return m_record->getMarkingPrefix() + buf;
	}
	mystring getGoodText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n ++ ++ Good !! ++ ++", getQ().c_str());
		return m_record->getMarkingPrefix() + buf;
	}
	mystring getBadText() const
	{
		wchar_t buf[1024];
		swprintf(buf, _countof(buf), L"%ls\r\n\r\n ++ ++ Bad ... ++ ++", getQ().c_str());
		return m_record->getMarkingPrefix() + buf;
	}
};
