#ifndef EVENT_IMPL_H
#define EVENT_IMPL_H

#include "ref-count.h"
#include "allocator.h"
#include "console-print.h"

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

	inline static void* operator new(size_t n);
	inline static void  operator delete(void* p, size_t n);

private:
	typedef FreeListCache Allocator_t;
	static Allocator_t allocator;
};

;

void* EventImpl::operator new(size_t n)
{
	OUT_MSG("EventImpl::operator new size:" << n);
	return SyncNone<FreeListCache>::Allocate(n);
}


void EventImpl::operator delete(void* p, size_t n)
{

	
	OUT_MSG("EventImpl::operator del size:" << n);
	SyncNone<FreeListCache>::Deallocate(p, n);
}



}

#endif