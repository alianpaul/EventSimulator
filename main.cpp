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

using namespace eventsim;

class MyStruct : public RefCount<MyStruct>
{
public:
	void SetI(int v) { m_i = v; }
	int  GetI() const { return m_i; }
	float Cal(int i, float& j, int k) const 
	{
		j = 0.1;
		return m_i + i + j + k;
	}
	int m_i;
};


int main()
{
	//Ptr<AttributeValue>    pv(new IntegerValue(2));
	//Ptr<AttributeChecker>  pc = MakeIntegerChecker<int32_t>();
	//Ptr<AttributeAccessor> pa = MakeIntegerAccessor(&MyStruct::GetI, &MyStruct::SetI);

}