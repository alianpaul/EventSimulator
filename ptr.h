#ifndef PTR_H
#define PTR_H

namespace eventsim
{

template<typename T>
class Ptr
{
public:
	template<typename U> 
	friend class Ptr;

	/* Help normal function to get the private m_ptr.
	*/
	template<typename U>
	friend U* PeekPointer(const Ptr<U>& p);

	/*Initiate a null pointer
	*/
	Ptr();
	/*Initiate a smart pointer from a raw pointer.
	* Invoke the underlying object's Ref method.
	*/
	Ptr(T* rawPtr);
	/*Invoke the underlying object's UnRef method.
	*/
	~Ptr();
	/*Copy ctor, Initiate a smart pointer from another smart pointer of the same 
	* type.
	*/
	Ptr(const Ptr& rhs);

	/*Copy ctor, Initiate from a different type
	*/
	template<typename U>
	Ptr(const Ptr<U>& rhs);
	/*Copy assign. Ptr object just like a pointer, no need to use copy-swap.
	* Attention: there is no need to use Move-ctor and Move-assign, because they are same with the copy conter-part.
	* we still need to call Acquire to make the move-version work correct, 
	*/
	Ptr& operator = (const Ptr& rhs);

	
	/*Const pointer, we can not change the pointer, wheather we can change the pointed obj depends on T.
	* Both const Ptr type and Ptr type obj use this version.
	*/
	T* operator -> () const;

	/* 
	*/
	T& operator *  () const;

	/*Test for nullptr
	*/
	bool operator ! () const;

private:
	inline void Acquire();

private:
	T* m_ptr;
};

template<typename T1, typename T2>
bool operator == (const Ptr<T1>& lhs, const Ptr<T2>& rhs);
template<typename T1, typename T2>
bool operator == (const Ptr<T1>& lhs, T2* rhs);
template<typename T1, typename T2>
bool operator == (T1* lhs, const Ptr<T2>& rhs);

template<typename T1, typename T2>
bool operator != (const Ptr<T1>& lhs, const Ptr<T2>& rhs);
template<typename T1, typename T2>
bool operator != (const Ptr<T1>& lhs, T2* rhs);
template<typename T1, typename T2>
bool operator != (T1* lhs, const Ptr<T2>& rhs);

template<typename T1, typename T2>
bool operator < (const Ptr<T1>& lhs, const Ptr<T2>& rhs);
template<typename T1, typename T2>
bool operator < (const Ptr<T1>& lhs, T2* rhs);
template<typename T1, typename T2>
bool operator < (T1* lhs, const Ptr<T2>& rhs);

template<typename T1, typename T2>
bool operator <= (const Ptr<T1>& lhs, const Ptr<T2>& rhs);
template<typename T1, typename T2>
bool operator <= (const Ptr<T1>& lhs, T2* rhs);
template<typename T1, typename T2>
bool operator <= (T1* lhs, const Ptr<T2>& rhs);

template<typename T1, typename T2>
bool operator > (const Ptr<T1>& lhs, const Ptr<T2>& rhs);
template<typename T1, typename T2>
bool operator > (const Ptr<T1>& lhs, T2* rhs);
template<typename T1, typename T2>
bool operator > (T1* lhs, const Ptr<T2>& rhs);

template<typename T1, typename T2>
bool operator >= (const Ptr<T1>& lhs, const Ptr<T2>& rhs);
template<typename T1, typename T2>
bool operator >= (const Ptr<T1>& lhs, T2* rhs);
template<typename T1, typename T2>
bool operator >= (T1* lhs, const Ptr<T2>& rhs);

/***Factory methods to easy the creation of the referenced object and Ptr***/
/*
* T: explicit, the type of the object to create
*/
template<typename T>
Ptr<T> Create();

/*
* T: explicit, the type of the object to create
* T0: deduced arg type, used in the construction of the object
*/
template<typename T, typename T0>
Ptr<T> Create(T0 a0);

template<typename T, typename T0, typename T1>
Ptr<T> Create(T0 a0, T1 a1);

template<typename T, typename T0, typename T1, typename T2>
Ptr<T> Create(T0 a0, T1 a1, T2 a2);

template<typename T, typename T0, typename T1, typename T2, typename T3>
Ptr<T> Create(T0 a0, T1 a1, T2 a2, T3 a3);

/*************IMPLEMENTATION OF Ptr TEMPLATE MEMBER FUNCTION*********/

template<typename T>
Ptr<T>::Ptr() : m_ptr(nullptr)
{
}

template<typename T>
Ptr<T>::Ptr(T* rawptr) : m_ptr(rawptr)
{
	Acquire();
}

template<typename T>
Ptr<T>::Ptr(const Ptr& rhs) : m_ptr(rhs.m_ptr)
{
	Acquire();
}

template<typename T>
template<typename U>
Ptr<T>::Ptr(const Ptr<U>& rhs) : m_ptr(rhs.m_ptr)
{
	Acquire();
}

template<typename T>
Ptr<T>&  Ptr<T>::operator = (const Ptr& rhs)
{
	if (&rhs == this)
		return *this;
	
	if (m_ptr)
	{
		m_ptr->UnRef();
	}

	m_ptr = rhs.m_ptr;
	Acquire();
	return *this;
}

template<typename T>
Ptr<T>::~Ptr()
{
	if (m_ptr)
	{
		m_ptr->UnRef();
	}
}

template<typename T>
T* Ptr<T>::operator -> () const
{
	return m_ptr;
}

template<typename T>
T& Ptr<T>::operator *  () const
{
	return *m_ptr;
}

template<typename T>
bool Ptr<T>::operator ! () const
{
	return m_ptr == nullptr;
}

template<typename T>
void Ptr<T>::Acquire()
{
	if (m_ptr)
	{
		m_ptr->Ref();
	}
}

/***********IMPLEMENTATION OF NORMAL FUNCTIONS**************/
template<typename T>
inline T* PeekPointer(const Ptr<T>& p)
{
	return p.m_ptr;
}

template<typename T1, typename T2>
bool operator == (const Ptr<T1>& lhs, const Ptr<T2>& rhs)
{
	return PeekPointer(lhs) == PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator == (const Ptr<T1>& lhs, T2* rhs)
{
	return PeekPointer(lhs) == rhs;
}

template<typename T1, typename T2>
bool operator == (T1* lhs, const Ptr<T2>& rhs)
{
	return lhs == PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator != (const Ptr<T1>& lhs, const Ptr<T2>& rhs)
{
	return PeekPointer(lhs) != PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator != (const Ptr<T1>& lhs, T2* rhs)
{
	return PeekPointer(lhs) != rhs;
}

template<typename T1, typename T2>
bool operator != (T1* lhs, const Ptr<T2>& rhs)
{
	return lhs != PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator < (const Ptr<T1>& lhs, const Ptr<T2>& rhs)
{
	return PeekPointer(lhs) < PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator < (const Ptr<T1>& lhs, T2* rhs)
{
	return PeekPointer(lhs) < rhs;
}

template<typename T1, typename T2>
bool operator < (T1* lhs, const Ptr<T2>& rhs)
{
	return lhs < PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator <= (const Ptr<T1>& lhs, const Ptr<T2>& rhs)
{
	return PeekPointer(lhs) <= PeekPointer(rhs);
}
template<typename T1, typename T2>
bool operator <= (const Ptr<T1>& lhs, T2* rhs)
{
	return PeekPointer(lhs) <= rhs;
}

template<typename T1, typename T2>
bool operator <= (T1* lhs, const Ptr<T2>& rhs)
{
	return lhs <= PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator > (const Ptr<T1>& lhs, const Ptr<T2>& rhs)
{
	return PeekPointer(lhs) > PeekPointer(rhs);
}
template<typename T1, typename T2>
bool operator > (const Ptr<T1>& lhs, T2* rhs)
{
	return PeekPointer(lhs) > rhs;
}
template<typename T1, typename T2>
bool operator > (T1* lhs, const Ptr<T2>& rhs)
{
	return lhs > PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator >= (const Ptr<T1>& lhs, const Ptr<T2>& rhs)
{
	return PeekPointer(lhs) >= PeekPointer(rhs);
}

template<typename T1, typename T2>
bool operator >= (const Ptr<T1>& lhs, T2* rhs)
{
	return PeekPointer(lhs) >= rhs;
}

template<typename T1, typename T2>
bool operator >= (T1* lhs, const Ptr<T2>& rhs)
{
	return lhs >= PeekPointer(rhs);
}

template<typename T>
Ptr<T> Create()
{
	return new T();
}

template<typename T, typename T0>
Ptr<T> Create(T0 a0)
{
	return new T(a0);
}

template<typename T, typename T0, typename T1>
Ptr<T> Create(T0 a0, T1 a1)
{
	return new T(a0, a1);
}

template<typename T, typename T0, typename T1, typename T2>
Ptr<T> Create(T0 a0, T1 a1, T2 a2)
{
	return new T(a0, a1, a2);
}

template<typename T, typename T0, typename T1, typename T2, typename T3>
Ptr<T> Create(T0 a0, T1 a1, T2 a2, T3 a3)
{
	return new T(a0, a1, a2, a3);
}

}

#endif