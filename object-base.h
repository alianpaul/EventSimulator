#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "type-id.h"

#define OBJECT_ENSURE_REGISTERED(type)                  \
  static struct Object ## type ## RegistrationClass     \
  {                                                     \
    Object ## type ## RegistrationClass () {            \
      TypeID tid = type::GetTypeID ();					\
      tid.AddSize (sizeof (type));                      \
    }                                                   \
  } Object ## type ## RegistrationVariable

namespace eventsim
{


class ObjectBase
{

public:
	/*Register the current class type info into the type system
	*/
	static TypeID GetTypeID();

	virtual ~ObjectBase();
};


}

#endif