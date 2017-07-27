#include "ptr.h"
#include "event-impl.h"
#include "make-event.h"
#include "console-print.h"
#include "allocator.h"

using namespace eventsim;

struct A : public RefCount<A>
{
	void func1(int a0)
	{
		OUT_MSG("A::func1 arg" << a0);
	}

	void func2(int a0, int a1)
	{
		OUT_MSG("A::func2 arg" << a0 << " " << a1);
	}
};

void func(int a0, int a1)
{
	OUT_MSG("Func " << a0 << " " << a1);
}

int main()
{
	Ptr<A> pa(new A);
	EventImpl* p = MakeEvent(&A::func2, pa, 10, 11);
	p->Notify();
	delete p;
	p = MakeEvent(&A::func1, pa, 12);
	p->Notify();
	delete p;
	
}