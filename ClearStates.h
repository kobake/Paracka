#pragma once

#include <StringLib.h>
#include <map>
class Record;

class ClearStates{
public:
	void putClearState(const Record& record, bool cleared);
	bool getClearState(const Record& record) const;
private:
	std::map<mystring, bool> m_states;
};
