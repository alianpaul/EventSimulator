#ifndef TYPE_ID_H
#define TYPE_ID_H

#include "attribute.h"
#include "ptr.h"
#include "callback.h"

#include <cstdint>

namespace eventsim
{

/*TypeID is the only interface to the type and attribute system.
* TypeID dispatch the work to a underlying singleton TypeInfoManager object.
* Other class that want to register its type information must hold a static TypeID
* member.
* 
* We must ensure that each type's explicit TypeID(const char*) ctor only called once through out
* the whole program(using function static variable). In this way we only register each class once.
*/
class TypeID
{
public:
	explicit TypeID(const char* classname);

	struct AttributeInformation
	{
		std::string						name;
		std::string						help;
		Ptr<const AttributeValue>		defaultValue;
		Ptr<const AttributeValue>		configValue;
		Ptr<const AttributeAccessor>	accessor;
		Ptr<const AttributeChecker>		checker;
	};

	TypeID&	AddAttribute	(std::string name,
							std::string help,
							Ptr<const AttributeValue> value,
							Ptr<const AttributeAccessor> accessor,
							Ptr<const AttributeChecker>  checker);

	template<typename T>
	TypeID& AddConstructor	();

	

	template<typename PARENT>
	TypeID& AddParent		();

	TypeID& AddSize			(size_t size);


private:
	/*Helper function to prevent client code from the TypeInfoManager object*/
	void	DoAddConstructor	(Callback<ObjectBase*> ctor);
	void	DoAddParent			(uint32_t pid);

	uint32_t m_tid;  
};

template<typename T>
TypeID& TypeID::AddConstructor()
{
	struct Ctor
	{
		static ObjectBase* Create()
		{
			return new T();
		}
	};

	Callback<ObjectBase*> ctorCB(Ctor::Create);
	DoAddConstructor(ctorCB);
	return *this;
}

template<typename PARENT>
TypeID& TypeID::AddParent()
{
	TypeID pid = PARENT::GetTypeID();
	DoAddParent(pid.m_tid);
	return *this;
}

}

#endif