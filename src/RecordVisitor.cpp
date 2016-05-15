#include "RecordVisitor.h"


// ユーティリティ
mystring trimPartMark(mystring text)
{
	//L"〔名〕";
	return CRegex::Replace(text.c_str(), L"〔.〕", L"");
}

// チェック
bool RecordVisitor::checkAnswer(mystring ans) const
{
	if(ans == getA())return true;
	if(trimPartMark(ans) == getA())return true;
	if(ans == trimPartMark(getA()))return true;
	return false;
}
