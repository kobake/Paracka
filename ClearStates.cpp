#include "ClearStates.h"
#include "Record.h"

void ClearStates::putClearState(const Record& record, bool cleared)
{
	m_states[record.m_q + record.m_a] = cleared;
}

bool ClearStates::getClearState(const Record& record) const
{
	std::map<mystring, bool>::const_iterator itr = m_states.find(record.m_q + record.m_a);
	if(itr != m_states.end()){
		return itr->second;
	}
	else{
		return false;
	}
}
