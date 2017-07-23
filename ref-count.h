#ifndef REF_COUNT_H
#define REF_COUNT_H

#include "console-print.h"
#include "default-deleter.h"

namespace eventsim{

template<typename T, typename DELETER = DefaultDeleter<T>>
class RefCount
{

public:
	inline void Ref() const
	{
		++m_count;
	};

	inline void UnRef() const
	{
		if (--m_count == 0)
		{
			DELETER::Delete(static_cast<T*>(const_cast<RefCount*>(this)));
		}
	};

	inline unsigned GetReferenceCount() const
	{
		return m_count;
	}

protected:
	RefCount() : m_count(0)
	{}
	RefCount(const RefCount &rhs) : m_count(0) 
	{}
	RefCount& operator=(const RefCount &rhs) 
	{ return *this; }

private:
	mutable unsigned m_count = 0;
};

}

#endif