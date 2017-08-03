#include "type-test-obj.h"
#include "integer-value.h"


namespace eventsim
{

OBJECT_ENSURE_REGISTERED(Fuck);

TypeID Fuck::GetTypeID()
{
	static TypeID id = TypeID("Fuck")
		.AddParent<ObjectBase>()
		.AddConstructor<Fuck>()
		.AddAttribute("i",
		"i of fuck",
		Create<IntegerValue>(10),
		MakeIntegerAccessor(&Fuck::m_i),
		MakeIntegerChecker<int>())
		.AddAttribute("j",
		"j of fuck",
		Create<IntegerValue>(20),
		MakeIntegerAccessor(&Fuck::GetJ, &Fuck::SetJ),
		MakeIntegerChecker<int>());
	return id;
}

int Fuck::GetJ() const
{
	return m_j;
}

void Fuck::SetJ(int j)
{
	m_j = j;
}

}