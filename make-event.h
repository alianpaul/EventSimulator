#ifndef MAKE_EVENT_H
#define MAKE_EVENT_H

#include "event-impl.h"

namespace eventsim
{

/*EventImplObjTraits only have 1 declartion and 2 partial specialization(For raw_ptr and Ptr),
* This way, we make sure that the OBJ must be raw pointer or a Ptr type.
*/
template<typename T> struct EventImplObjTraits;

template<typename T>
struct EventImplObjTraits<T*>
{
	static T& GetReference(T* p)
	{
		return *p;
	}
};

template<typename T>
struct EventImplObjTraits<Ptr<T>>
{
	static T& GetReference(Ptr<T> p)
	{
		return *p;
	}
};


/*********Factory functions declarations to make the EventImpl**********/
//Member function
template<typename MEM, typename OBJ>
EventImpl* MakeEvent(MEM mem_ptr, OBJ obj);

template<typename MEM, typename OBJ, typename ARG0>
EventImpl* MakeEvent(MEM mem_ptr, OBJ obj, ARG0 arg0);

template<typename MEM, typename OBJ, typename ARG0, typename ARG1>
EventImpl* MakeEvent(MEM mem_ptr, OBJ obj, ARG0 arg0, ARG1 arg1);

//Normal function
EventImpl* MakeEvent(void(*f)(void));

template<typename U0, typename V0>
EventImpl* MakeEvent(void(*f)(U0), V0 v0);

template<typename U0, typename U1, typename V0, typename V1>
EventImpl* MakeEvent(void(*f)(U0, U1), V0 v0, V1 v1);

/*********Factory functions implementations***********/
template<typename MEM, typename OBJ>
EventImpl* MakeEvent(MEM mem_ptr, OBJ obj)
{
	class EventImplMem0Arg : public EventImpl
	{
	public:
		EventImplMem0Arg(MEM mem_ptr, OBJ obj) :
			m_mem(mem_ptr), m_obj(obj)
		{}

		void Notify()
		{
			(EventImplObjTraits<OBJ>::GetReference(m_obj).*m_mem)();
		}

	private:
		MEM m_mem;
		OBJ m_obj;
	};

	return new EventImplMem0Arg(mem_ptr, obj);
}

template<typename MEM, typename OBJ, typename ARG0>
EventImpl* MakeEvent(MEM mem_ptr, OBJ obj, ARG0 arg0)
{
	class EventImplMem1Arg : public EventImpl
	{
	public:
		EventImplMem1Arg(MEM mem_ptr, OBJ obj, ARG0 arg0) :
			m_mem(mem_ptr), m_obj(obj),
			m_arg0(arg0)
		{}

		void Notify()
		{
			(EventImplObjTraits<OBJ>::GetReference(m_obj).*m_mem)(m_arg0);
		}

	private:
		MEM m_mem;
		OBJ m_obj;
		ARG0 m_arg0;
	};

	return new EventImplMem1Arg(mem_ptr, obj, arg0);
}

template<typename MEM, typename OBJ, typename ARG0, typename ARG1>
EventImpl* MakeEvent(MEM mem_ptr, OBJ obj, ARG0 arg0, ARG1 arg1)
{
	class EventImplMem2Arg : public EventImpl
	{
	public:
		EventImplMem2Arg(MEM mem_ptr, OBJ obj, ARG0 arg0, ARG1 arg1) :
			m_mem(mem_ptr), m_obj(obj),
			m_arg0(arg0),
			m_arg1(arg1)
		{}

		void Notify()
		{
			(EventImplObjTraits<OBJ>::GetReference(m_obj).*m_mem)(m_arg0, m_arg1);
		}

	private:
		MEM m_mem;
		OBJ m_obj;
		ARG0 m_arg0;
		ARG1 m_arg1;
	};

	return new EventImplMem2Arg(mem_ptr, obj, arg0, arg1);
}

EventImpl* MakeEvent(void(*f)(void))
{
	class EventImplFunc0Arg : public EventImpl
	{
	public:
		EventImplFunc0Arg(void(*f)(void)) :
			m_f(f)
		{}

		void Notify()
		{
			(*m_f)();
		}

	private:
		void(*m_f) (void);
	};

	return new EventImplFunc0Arg(f);
}

template<typename U0, typename V0>
EventImpl* MakeEvent(void(*f)(U0), V0 v0)
{
	class EventImplFunc1Arg : public EventImpl
	{
	public:
		EventImplFunc1Arg(void(*f)(U0), V0 v0) :
			m_f(f),
			m_v0(v0)
		{}

		void Notify()
		{
			(*m_f)(m_v0);
		}
	private:
		void(*m_f) (U0);
		V0 m_v0;
	};

	return new EventImplFunc1Arg(f, v0);
}

template<typename U0, typename U1, typename V0, typename V1>
EventImpl* MakeEvent(void(*f)(U0, U1), V0 v0, V1 v1)
{
	class EventImplFunc2Arg : public EventImpl
	{
	public:
		EventImplFunc2Arg(void(*f)(U0, U1), V0 v0, V1 v1) :
			m_f(f),
			m_v0(v0),
			m_v1(v1)
		{}

		void Notify()
		{
			(*m_f)(m_v0, m_v1);
		}

	private:
		void(*m_f) (U0, U1);
		V0 m_v0;
		V1 m_v1;
	};

	return new EventImplFunc2Arg(f, v0, v1);
}

}

#endif