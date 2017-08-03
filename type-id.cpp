#include "type-id.h"
#include "singleton.h"
#include "console-print.h"

#include <string>
#include <vector>
#include <map>

namespace eventsim
{

/*Type information*/
struct TypeInfo
{
	std::string									name;
	uint16_t									parent;
	size_t										size;
	Callback<ObjectBase*>                       constructor;
	std::vector<TypeID::AttributeInformation>	attributes;
};

/* Singleton class only used in TypeID member functions
*/
class TypeInfoManager : public Singleton<TypeInfoManager>
{

public:
	
	/*Add new type entry in the type container
	* return: id of the type. Attention: the real idx of the type in the type container is (id - 1)
	*/
	uint32_t	AllocateType(std::string name);

	TypeInfo&   GetTypeInfo (uint32_t tid);

	bool		HasAttribute(uint32_t tid, std::string attrname);

private:

	typedef std::vector<struct TypeInfo>	  TypeVec_t;
	typedef std::map<std::string, uint16_t>   NameMap_t;

	/*Runtime container of the type information*/
	TypeVec_t m_typeInfos;
	/*By name index*/
	NameMap_t  m_nameMap; 
};


/*Only called in TypeID(const char* classname),This function will be called only once
* for each specific class. To make sure this data invariant, validate the data first.
*/
uint32_t 
TypeInfoManager::AllocateType(std::string name)
{
	if (m_nameMap.count(name) != 0)
	{
		OUT_ERR("TypeInfoManager::AllocateType: Duplicate type to allocate");
		exit(-1);
	}

	TypeInfo newtype;
	newtype.name = name;
	newtype.parent = 0; //means no parent    
	newtype.size = 0;

	m_typeInfos.push_back(newtype);
	uint32_t id = m_typeInfos.size(); 
	//So the id of the types start from 1, 0 is reserved for no parent class's parent
	
	m_nameMap[name] = id; 
	return id;
}

TypeInfo& 
TypeInfoManager::GetTypeInfo(uint32_t tid)
{
	return m_typeInfos[tid - 1];
}

bool 
TypeInfoManager::HasAttribute(uint32_t tid, std::string attrname)
{
	TypeInfo& type = GetTypeInfo(tid);
	for (size_t i = 0; i < type.attributes.size(); ++i)
	{
		if (type.attributes[i].name == attrname)
			return true;
	}
	return false;
}

/*******************TypeID member functions implementations******************/
TypeID::TypeID(const char* classname)
{
	m_tid = TypeInfoManager::GetInstance()->AllocateType(classname);
}


void 
TypeID::DoAddConstructor(Callback<ObjectBase*> ctor)
{
	TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	type.constructor = ctor;
	return;
}

void 
TypeID::DoAddParent(uint32_t pid)
{
	TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	type.parent = pid;
	return;
}

TypeID& 
TypeID::AddAttribute(std::string name, std::string help,
					Ptr<const AttributeValue> value,
					Ptr<const AttributeAccessor> accessor,
					Ptr<const AttributeChecker>  checker)
{
	//Assert it's a new attribute to add
	_ASSERT(!TypeInfoManager::GetInstance()->HasAttribute(m_tid, name));

	AttributeInformation attrinfo;
	attrinfo.name			= name;
	attrinfo.help			= help;
	attrinfo.defaultValue	= value;
	attrinfo.accessor		= accessor;
	attrinfo.checker		= checker;

	TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	type.attributes.push_back(attrinfo);
	return *this;
}

TypeID&
TypeID::AddSize(size_t size)
{
	TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	type.size = size;
	return *this;
}

}