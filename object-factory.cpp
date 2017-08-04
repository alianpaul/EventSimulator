#include "object-factory.h"

namespace eventsim
{

ObjectFactory::ObjectFactory(std::string name)
{
	m_tid = TypeID::LookupByName(name);
	_ASSERT(m_tid.isValid());
}

Ptr<Object>
ObjectFactory::Create()		const
{
	ObjectBase* base = DoCreate();
	return Ptr<Object>(dynamic_cast<Object*>(base));
}

ObjectBase*
ObjectFactory::DoCreate()	const
{
	Callback<ObjectBase*> ctor = m_tid.GetConstructor();
	//Construct the object
	ObjectBase* base = ctor();
	//Set the attributes, from the current object parts all along to the ObjectBase
	for (TypeID tid = m_tid; tid != ObjectBase::GetTypeID(); tid = tid.GetParent())
	{
		for (size_t i = 0; i < tid.GetAttributeN(); ++i)
		{
			const TypeID::AttributeInformation& attr = tid.GetAttribute(i);
			attr.checker->Check	((*attr.defaultValue));
			attr.accessor->Set	(base, (*attr.defaultValue));
		}
	}

	return base;
}

}