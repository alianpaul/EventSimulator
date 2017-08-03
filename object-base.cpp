#include "object-base.h"



namespace eventsim
{

OBJECT_ENSURE_REGISTERED(ObjectBase);

TypeID ObjectBase::GetTypeID()
{
	static TypeID id = TypeID("ObjectBase")
		.AddConstructor<ObjectBase>();
	return id;
}

ObjectBase::~ObjectBase()
{}

}