#ifndef TYPE_TEST_OBJ_H
#define TYPE_TEST_OBJ_H

#include "object-base.h"


namespace eventsim
{
struct Fuck : public ObjectBase
{
	Fuck(){};
	static TypeID GetTypeID();

	int  GetJ() const;
	void SetJ(int j);

	int m_i;
	int m_j;
};

}

#endif