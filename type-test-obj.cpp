#include "type-test-obj.h"
#include "integer-value.h"


namespace eventsim
{

OBJECT_ENSURE_REGISTERED(Base0);

TypeID Base0::GetTypeID()
{
	static TypeID id = 
		TypeID("Base0")
		.AddParent<Object>()
		.AddConstructor<Base0>()
		.AddAttribute("i",
					"i of fuck",
					Create<IntegerValue>(10),
					MakeIntegerAccessor(&Base0::m_i),
					MakeIntegerChecker<int>())
		.AddAttribute("j",
					"j of fuck",
					Create<IntegerValue>(20),
					MakeIntegerAccessor(&Base0::GetJ, &Base0::SetJ),
					MakeIntegerChecker<int>());
	return id;
}

int Base0::GetJ() const
{
	return m_j;
}

void Base0::SetJ(int j)
{
	m_j = j;
}

}