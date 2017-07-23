#include "scheduler.h"

namespace eventsim
{

void 
Scheduler::Insert(const Event& e)
{
	std::pair<EventIT, bool> result =  m_events.insert(std::make_pair(e.key, e.impl));
	_ASSERT(result.second);
}

void 
Scheduler::RemoveNext()
{
	EventIT bi = m_events.begin();
	_ASSERT(bi != m_events.end());
	m_events.erase(bi);
}

Scheduler::Event
Scheduler::PeekNext() const
{
	EventCIT bci = m_events.cbegin();
	_ASSERT(bci != m_events.end());
	Event result;
	result.key	= bci->first;
	result.impl = bci->second;
	return result;
}

bool
Scheduler::IsEmpty() const
{
	return m_events.empty();
}


}