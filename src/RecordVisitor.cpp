#include "RecordVisitor.h"


// ���[�e�B���e�B
mystring trimPartMark(mystring text)
{
	//L"�k���l";
	return CRegex::Replace(text.c_str(), L"�k.�l", L"");
}

// �`�F�b�N
bool RecordVisitor::checkAnswer(mystring ans) const
{
	if(ans == getA())return true;
	if(trimPartMark(ans) == getA())return true;
	if(ans == trimPartMark(getA()))return true;
	return false;
}
