#ifndef EVENT_IMPL_H
#define EVENT_IMPL_H

#include "ref-count.h"

namespace eventsim
{

class EventImpl : public RefCount<EventImpl>
{

public:
	/* Excute the stored function.
	*/
	EventImpl(){}
	virtual ~EventImpl(){}
	virtual void Notify() = 0;
};


}

#endif