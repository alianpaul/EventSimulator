#include "ptr.h"
#include "event-impl.h"
#include "make-event.h"
#include "console-print.h"

using namespace eventsim;

struct A : public RefCount<A>
{
	void func(int a0, int a1)
	{
		OUT_MSG("Fuck " << a0 << " " << a1);
	}
};

void func(int a0, int a1)
{
	OUT_MSG("Func " << a0 << " " << a1);
}

int main()
{
	
	EventImpl * ep = MakeEvent(&func, 10, 11);
	ep->Notify();
}