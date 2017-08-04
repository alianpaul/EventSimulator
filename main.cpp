#include "ptr.h"
#include "event-impl.h"
#include "make-event.h"
#include "console-print.h"
#include "allocator.h"
#include "type-id.h"
#include "singleton.h"

#include "integer-value.h"
#include "attribute-accessor-helper.h"
#include "object-base.h"
#include "callback.h"
#include "object.h"

#include "type-test-obj.h"
#include "object-factory.h"

using namespace eventsim;



struct Derived : public Base0
{
	static TypeID GetTypeID()
	{
		static TypeID tid =
			TypeID("Derived")
			.AddConstructor<Derived>()
			.AddParent<Base0>()
			.AddAttribute("mdi",
				"m di of Derived",
				Create<IntegerValue>(100),
				MakeIntegerAccessor(&Derived::m_di),
				MakeIntegerChecker<int>());
		return tid;
	}

	int m_di;
};

OBJECT_ENSURE_REGISTERED(Derived);

int main()
{
	ObjectFactory factory("Derived");
}