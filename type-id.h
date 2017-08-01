#ifndef TYPE_ID_H
#define TYPE_ID_H

#include <cstdint>

namespace eventsim
{



class TypeID
{
public:
	explicit TypeID(const char* classname);

private:
	uint16_t m_tid;
};

}

#endif