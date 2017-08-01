
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "ref-count.h"
#include <string>

namespace eventsim
{

class ObjectBase;

/*AttributeValue is a interface class, An implemented AttributeValue object HOLDS a value of the attribute.
* Implemented AttributeValue type should also have Set, Get function. We don't put these two here because of 
* the type problem.
*/
class AttributeValue : public RefCount<AttributeValue>
{
public:
	AttributeValue();
	virtual ~AttributeValue();

	virtual std::string SerializeToString() const = 0;
	virtual bool		DeserializeFromString(std::string value) = 0;
};

/*AttributeAccessor 
*/
class AttributeAccessor : public RefCount<AttributeAccessor>
{
public:
	AttributeAccessor();
	virtual ~AttributeAccessor();

	virtual bool Set(		ObjectBase * object, const	AttributeValue& value) const = 0;

	virtual bool Get(const	ObjectBase * object,		AttributeValue& value) const = 0;
};

/*AttributeChecker associated with the real type of the attribute
*/
class AttributeChecker : public RefCount<AttributeChecker>
{
public:
	AttributeChecker();
	virtual ~AttributeChecker();

	virtual bool Check(const AttributeValue& value) const = 0;
};

}

#endif