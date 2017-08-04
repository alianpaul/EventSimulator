#include "object.h"

namespace eventsim
{

OBJECT_ENSURE_REGISTERED(Object);

TypeID
Object::GetTypeID()
{
	static TypeID tid =
		TypeID("Object")
		.AddParent<ObjectBase>()
		.AddSize(sizeof(Object));  //Can not construct Object object

	return tid;
}

}