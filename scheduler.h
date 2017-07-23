#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <cstdint>
#include <map>

namespace eventsim
{

class EventImpl;

class Scheduler
{

public:
	struct EventKey
	{
		uint64_t ts; //Time
		uint32_t id; //eventID
	};

	struct Event
	{
		EventKey	key;
		EventImpl*	impl;
	};

	void	Insert		(const Event& e);
	Event	PeekNext	() const;
	void	RemoveNext	();
	bool	IsEmpty		() const;

private:

	typedef std::map<EventKey, EventImpl*>	EventQueue;
	typedef EventQueue::iterator			EventIT;
	typedef EventQueue::const_iterator		EventCIT;

	EventQueue m_events;
};

}

#endif