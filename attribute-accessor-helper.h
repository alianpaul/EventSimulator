#ifndef ATTRIBUTE_ACCESSOR_HELPER_H
#define ATTRIBUTE_ACCESSOR_HELPER_H

#include "attribute.h"
#include "ptr.h"

namespace eventsim
{

/*OBJ: The class of object holding the attribute.
* ATTV: The implemented AttributeValue type.
*/
template<typename OBJ, typename ATTV>
class AccessorHelper : public AttributeAccessor
{
public:
	AccessorHelper() {}

	bool Set(ObjectBase * object, const AttributeValue& value) const
	{
		const ATTV *pv = dynamic_cast<const ATTV *>(&value);
		if (pv == 0)
		{
			return false;
		}

		OBJ *po = dynamic_cast<OBJ*>(object);
		if (po == 0)
		{
			return false;
		}

		return DoSet(po, pv);
	}

	bool Get(const ObjectBase * object, AttributeValue& value) const
	{
		ATTV *pv = dynamic_cast<ATTV *>(&value);
		if (pv == 0)
		{
			return false;
		}

		const OBJ* po = dynamic_cast<const OBJ*>(object);
		if (po == 0)
		{
			return false;
		}

		return DoGet(po, pv);
	}

private:
	virtual bool DoSet(OBJ* object, const	ATTV* value) const = 0;
	virtual bool DoGet(const	OBJ* object, ATTV* value) const = 0;
};


/*ATTV: explicit, The type of the attribute value, we decoupled the attribute value from the real attribute type of
*		the object. we need to specify it explicitly when we implement different AttributeValue(A wrap factory
*		method)
* OBJ:  deduced, the type of the object to access
* MEM:  deduced, the type of the object data memberfunction
*/
template<typename ATTV, typename OBJ, typename MEM>
Ptr<AttributeAccessor>
MakeAccessor(MEM OBJ::* memVar)
{
	class MemberVariable : public AccessorHelper<OBJ, ATTV>
	{
	public:
		MemberVariable(MEM OBJ::* memVar) : m_memVar(memVar)
		{}

		bool DoSet(OBJ* object, const ATTV* value) const
		{
			object->*m_memVar = value->Get();
			return true;
		}

		bool DoGet(const OBJ* object, ATTV* value) const
		{
			value->Set(object->*m_memVar);
			return true;
		}

	private:
		MEM OBJ::* m_memVar;
	};

	return new MemberVariable(memVar);
}

/*ATTV: explicit, the type of the attribute value
* OBJ:  deduced, the type of the object to access
* RET:	deduced, the return type of the attribute Getter function
* ARG:  deduced, the argument type of the attribute Setter function
*/
template<typename ATTV, typename OBJ, typename RET, typename ARG>
Ptr<AttributeAccessor>
MakeAccessor( RET	(OBJ::*getter)(void) const,
			  void	(OBJ::*setter)(ARG))
{

	class MemberMethod : public AccessorHelper<OBJ, ATTV>
	{
	public:
		MemberMethod(RET(OBJ::*getter)(void) const, void (OBJ::*setter)(ARG)) :
			m_getter(getter),
			m_setter(setter)
		{}

		bool DoSet(OBJ* object, const ATTV* value) const
		{
			(object->*m_setter)(value->Get());
			return true;
		}

		bool DoGet(const OBJ* object, ATTV* value) const
		{
			value->Set((object->*m_getter)());
			return true;
		}

	private:
		RET(OBJ::*m_getter)(void) const;
		void (OBJ::*m_setter)(ARG);
	};

	return new MemberMethod(getter, setter);

}

}

#endif