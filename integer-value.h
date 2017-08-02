#ifndef INTEGER_VALUE_H
#define INTEGER_VALUE_H

#include "attribute.h"
#include "attribute-accessor-helper.h"
#include "ptr.h"
#include <cstdint>
#include <limits>

namespace eventsim
{

/* IntegerValue is the AttributeValue that holds integer.
*/

class IntegerValue : public AttributeValue
{
public:
	IntegerValue();
	IntegerValue(const int64_t& value);

	void	Set(const int64_t& value);
	int64_t Get()	const;

	std::string SerializeToString()	const					override;
	bool		DeserializeFromString(std::string value)	override;

private:
	int64_t m_value;
};

class IntegerChecker : public AttributeChecker
{
public:
	IntegerChecker(int64_t min, int64_t max) :
		m_min(min),
		m_max(max)
	{}

	bool Check(const AttributeValue& value) const
	{
		const IntegerValue * pv = dynamic_cast<const IntegerValue *>(&value);
		if (pv == 0)
		{
			return false;
		}

		return pv->Get() >= m_min && pv->Get() <= m_max;
	}

private:
	int64_t m_min;
	int64_t m_max;
};

/* T: explicit, The type of the attribute, int16_t, int32_t, int64_t
*/
template<typename T>
Ptr<AttributeChecker>
MakeIntegerChecker(	int64_t min = std::numeric_limits<T>::min(), 
					int64_t max = std::numeric_limits<T>::max())
{
	return new IntegerChecker(min, max);
}

template<typename OBJ, typename RET, typename ARG>
Ptr<AttributeAccessor>
MakeIntegerAccessor(RET		(OBJ::*getter)(void) const,
					void	(OBJ::*setter)(ARG))
{
	return MakeAccessor<IntegerValue>(getter, setter);
}


template<typename OBJ, typename MEM>
Ptr<AttributeAccessor>
MakeIntegerAccessor(MEM OBJ::*memVar)
{
	return MakeAccessor<IntegerValue>(memVar);
}

}

#endif