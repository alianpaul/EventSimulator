#ifndef TYPE_TEST_OBJ_H
#define TYPE_TEST_OBJ_H

#include "object.h"

namespace eventsim
{
struct Base0 : public Object
{
	Base0(){};
	static TypeID GetTypeID();

	int  GetJ() const;
	void SetJ(int j);

	int m_i;
	int m_j;
};

}

#endif