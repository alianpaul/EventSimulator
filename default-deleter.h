#ifndef DEFAULT_DELETER_H
#define DEFAULT_DELETER_H

namespace eventsim
{

template<typename T>
struct DefaultDeleter
{
	inline static void Delete(T* pobj)
	{
		delete pobj;
	}
};

}

#endif