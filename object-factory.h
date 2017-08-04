#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <string>

#include "type-id.h"
#include "ptr.h"
#include "object.h"

namespace eventsim
{

class ObjectFactory
{
public:
	explicit ObjectFactory(std::string name);

	Ptr<Object> Create()	const;

	template<typename T>
	Ptr<T>		Create()	const;

private:
	ObjectBase* DoCreate()	const;

	TypeID	m_tid;
};

template<typename T>
Ptr<T>
ObjectFactory::Create() const
{
	ObjectBase* base = DoCreate();
	T*			derv = dynamic_cast<T*>(base);
	return Ptr<T>(derv);
}

}

#endif