
#include "integer-value.h"
#include <sstream>

namespace eventsim
{

IntegerValue::IntegerValue() : m_value()
{}

IntegerValue::IntegerValue(const int64_t& value) : m_value(value)
{}

void IntegerValue::Set(const int64_t& value)
{
	m_value = value;
}

int64_t IntegerValue::Get() const
{
	return m_value;
}

std::string IntegerValue::SerializeToString() const
{
	std::ostringstream oss;
	oss << m_value;
	return oss.str();
}

bool IntegerValue::DeserializeFromString(std::string value)
{
	std::istringstream iss;
	iss.str(value);
	iss >> m_value;
	return !iss.bad() && iss.fail();
}

}