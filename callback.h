
#include "ref-count.h"
#include "ptr.h"

namespace eventsim
{

struct Empty{};

/*This base class provides RefCount function to Callback templates
*/
class CallbackImplBase : public RefCount<CallbackImplBase>
{
public:
	virtual ~CallbackImplBase(){};
};


template<typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class CallbackImpl : public CallbackImplBase
{
public:
	virtual ~CallbackImpl(){};
	virtual R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) const = 0;
};

/*Partial specialization of orignal CallbackImpl template
* We use partial specialization technique to differ the CallbackImpl operator()() interface.
* In this way, we don't need to define a big CallbackImpl template to include all interfaces(operator()with diff
* number of arguments). We call these implemented interface in Callback through a CallbackImpl Ptr. It's more 
* safer to control the interface we have.
*/
template<typename R>
class CallbackImpl<R, Empty, Empty, Empty, Empty, Empty> : public CallbackImplBase
{
public:
	virtual ~CallbackImpl() {};
	virtual R operator()() const = 0;
};

template<typename R, typename T0>
class CallbackImpl<R, T0, Empty, Empty, Empty, Empty> : public CallbackImplBase
{
public:
	virtual ~CallbackImpl() {};
	virtual R operator()(T0 a0) const = 0;
};

template<typename R, typename T0, typename T1>
class CallbackImpl<R, T0, T1, Empty, Empty, Empty> : public CallbackImplBase
{
public:
	virtual ~CallbackImpl() {};
	virtual R operator()(T0 a0, T1 a1) const = 0;
};

template<typename R, typename T0, typename T1, typename T2>
class CallbackImpl<R, T0, T1, T2, Empty, Empty> : public CallbackImplBase
{
public:
	virtual ~CallbackImpl() {};
	virtual R operator()(T0 a0, T1 a1, T2 a2) const = 0;
};

template<typename R, typename T0, typename T1, typename T2, typename T3>
class CallbackImpl<R, T0, T1, T2, T3, Empty> : public CallbackImplBase
{
public:
	virtual ~CallbackImpl() {};
	virtual R operator()(T0 a0, T1 a1, T2 a2, T3 a3) const = 0;
};

/*The Functor implementation of CallbackImpl class(template).
* We provide implementation of the choosen interface. Different number of Empty type template arguments
* leads us to different base template to inherit. In this way, we choose a correct operator() interface to implement.
* Other operator()s are not virtual because the Base class we choose,so they can not be called through a CallbackImpl
* Ptr.
* P.S. Compiler only implement the template member function that be called. So don't bother with the mathing problem 
* in other functions
*/
template<typename FUNC, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncCallbackImpl : public CallbackImpl<R, T0, T1, T2, T3, T4>
{

public:
	FuncCallbackImpl(FUNC func) : m_functor(func)
	{}
	virtual ~FuncCallbackImpl(){}

	R operator()() const
	{
		return (m_functor)();
	}
	
	R operator()(T0 a0) const
	{
		return (m_functor)(a0);
	}

	R operator()(T0 a0, T1 a1) const
	{
		return (m_functor)(a0, a1);
	}

	R operator()(T0 a0, T1 a1, T2 a2) const
	{
		return (m_functor)(a0, a1, a2);
	}

	R operator()(T0 a0, T1 a1, T2 a2, T3 a3) const
	{
		return (m_functor)(a0, a1, a2, a3);
	}

	R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) const
	{
		return (m_functor)(a0, a1, a2, a3, a4);
	}
	
private:
	FUNC m_functor;
};

/*The member function implementation of CallbackImpl class(template).
* We use CallbackTraits to ensure that OBJ_PTR is a raw pointer to an object.
* If it's not a raw pointer, we can get a precise error.
*/
template<typename T>
struct CallbackTraits;

template<typename T>
struct CallbackTraits<T*>
{
	static T& GetReference(T* p)
	{
		return *p;
	}
};

template<typename OBJ_PTR, typename MEM_FUNC, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class MemFuncCallbackImpl : public CallbackImpl<R, T0, T1, T2, T3, T4>
{
public:
	MemFuncCallbackImpl(OBJ_PTR pobj, MEM_FUNC pmem) : 
		m_pobj(pobj),
		m_pmem(pmem)
	{}

	R operator()() const
	{
		return (CallbackTraits<OBJ_PTR>::GetReference(m_pobj).*m_pmem)();
	}

	R operator()(T0 a0) const
	{
		return (CallbackTraits<OBJ_PTR>::GetReference(m_pobj).*m_pmem)(a0);
	}

	R operator()(T0 a0, T1 a1) const
	{
		return (CallbackTraits<OBJ_PTR>::GetReference(m_pobj).*m_pmem)(a0, a1);
	}

	R operator()(T0 a0, T1 a1, T2 a2) const
	{
		return (CallbackTraits<OBJ_PTR>::GetReference(m_pobj).*m_pmem)(a0, a1, a2);
	}

	R operator()(T0 a0, T1 a1, T2 a2, T3 a3) const
	{
		return (CallbackTraits<OBJ_PTR>::GetReference(m_pobj).*m_pmem)(a0, a1, a2, a3);
	}

	R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) const
	{
		return (CallbackTraits<OBJ_PTR>::GetReference(m_pobj).*m_pmem)(a0, a1, a2, a3, a4);
	}

private:
	OBJ_PTR  m_pobj;
	MEM_FUNC m_pmem;
};

/*This demo only support at most 5 different arguments.
*/
template<typename R,	
		typename T0 = Empty, typename T1 = Empty,	
		typename T2 = Empty, typename T3 = Empty, 
		typename T4 = Empty>
class Callback
{
public:
	
	typedef CallbackImpl<R, T0, T1, T2, T3, T4>   CallbackImpl_t;

	/*Construct a functor callback. 
	* FUNC: deduced, the ref and const will be removed
	*/
	template<typename FUNC>
	Callback(FUNC func):
		m_impl(Create<FuncCallbackImpl<FUNC, R, T0, T1, T2, T3, T4>>(func))
	{}

	/*Construct a object mem_func callback
	* OBJ_PTR: deduced, the ref and const will be removed
	* MEM_FUNC: deduced, the ref and const will be removed
	*/
	template<typename OBJ_PTR, typename MEM_FUNC>
	Callback(OBJ_PTR pobj, MEM_FUNC pmem):
		m_impl(Create<MemFuncCallbackImpl<OBJ_PTR, MEM_FUNC, R, T0, T1, T2, T3, T4>>(pobj, pmem))
	{}

	R operator()() const
	{
		return (*m_impl)();
	}

	R operator()(T0 a0) const
	{
		return (*m_impl)(a0);
	}

	R operator()(T0 a0, T1 a1) const
	{
		return (*m_impl)(a0, a1);
	}

	R operator()(T0 a0, T1 a1, T2 a2) const
	{
		return (*m_impl)(a0, a1, a2);
	}

	R operator()(T0 a0, T1 a1, T2 a2, T3 a3) const
	{
		return (*m_impl)(a0, a1, a2, a3);
	}

	R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) const
	{
		return (*m_impl)(a0, a1, a2, a3, a4);
	}

private:
	Ptr<CallbackImpl_t> m_impl;
};


}