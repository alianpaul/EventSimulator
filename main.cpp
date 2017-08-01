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

using namespace eventsim;

class MyStruct : public ObjectBase
{
public:
	void SetI(int v) { m_i = v; }
	int  GetI() const { return m_i; }
	int m_i;
};



int main()
{
	Ptr<AttributeValue>    pv(new IntegerValue(2));
	Ptr<AttributeChecker>  pc = MakeIntegerChecker<int32_t>();
	Ptr<AttributeAccessor> pa = MakeIntegerAccessor(&MyStruct::GetI, &MyStruct::SetI);

}