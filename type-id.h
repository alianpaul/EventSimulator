#ifndef TYPE_ID_H
#define TYPE_ID_H

#include "attribute.h"
#include "ptr.h"

#include <cstdint>

namespace eventsim
{



class TypeID
{
public:
	explicit TypeID(const char* classname);

	struct AttributeInformation
	{
		std::string						name;
		std::string						help;
		Ptr<const AttributeValue>		defaultValue;
		Ptr<const AttributeValue>		configValue;
		Ptr<const AttributeAccessor>	accessor;
		Ptr<const AttributeChecker>		checker;
	};

	TypeID& AddAttribute(std::string name,
						 std::string help,
						 const AttributeValue& value,
						 Ptr<const AttributeAccessor> accessor,
						 Ptr<const AttributeChecker>  checker);

	/*Get the number of attributes of this class
	*/
	uint32_t					GetAttributeN() const;
	/*Get an attribute according to it's index.
	*/
	struct AttributeInformation GetAttribute(uint32_t i) const;

private:
	uint32_t m_tid;
};

}

#endif