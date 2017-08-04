#ifndef OBJECT_H
#define OBJECT_H

#include "ref-count.h"
#include "object-base.h"

namespace eventsim
{

/*Combine the type attribute system with the refcount function.
* Inherit from Object will have both function
*/
class Object : public ObjectBase, public RefCount<Object>
{
public:
	static TypeID GetTypeID();
	virtual ~Object(){}
};

}

#endif