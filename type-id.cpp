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
	uint32_t									parent;
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

	TypeInfo&   GetTypeInfo (uint32_t tid) const;

	bool		HasAttribute(uint32_t tid, std::string attrname) const;

	uint32_t	GetTid(std::string name) const;

private:

	typedef std::vector<struct TypeInfo>	  TypeVec_t;
	typedef std::map<std::string, uint16_t>   NameMap_t;

	/*Runtime container of the type information*/
	mutable TypeVec_t m_typeInfos;
	/*By name index*/
	mutable NameMap_t  m_nameMap; 
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
	newtype.parent = 0; //means no parent, parent is a invalid type    
	newtype.size = 0;

	m_typeInfos.push_back(newtype);
	uint32_t id = m_typeInfos.size(); 
	//So the id of the types start from 1, 0 is reserved for no parent class's parent
	
	m_nameMap[name] = id; 
	return id;
}

TypeInfo& 
TypeInfoManager::GetTypeInfo(uint32_t tid) const
{
	return m_typeInfos[tid - 1];
}

bool 
TypeInfoManager::HasAttribute(uint32_t tid, std::string attrname) const
{
	TypeInfo& type = GetTypeInfo(tid);
	for (size_t i = 0; i < type.attributes.size(); ++i)
	{
		if (type.attributes[i].name == attrname)
			return true;
	}
	return false;
}

uint32_t
TypeInfoManager::GetTid(std::string name) const
{
	NameMap_t::const_iterator it = m_nameMap.find(name);
	uint32_t tid;
	if (it != m_nameMap.cend())
	{
		tid = it->second;
	}
	else
	{
		tid = 0; //invalid tid
	}
	return tid;
}

/*******************TypeID member functions implementations******************/
/*Public static method, Called by client to get the TypeID
*/
TypeID TypeID::LookupByName(std::string name)
{
	uint32_t tid = TypeInfoManager::GetInstance()->GetTid(name);
	return TypeID(tid);
}

TypeID::TypeID(const char* classname)
{
	m_tid = TypeInfoManager::GetInstance()->AllocateType(classname);
}

bool
TypeID::isValid() const
{
	return m_tid != 0;
}

bool
TypeID::operator!=(const TypeID& o) const
{
	return m_tid != o.m_tid;
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

/*Attribute functions*/
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

size_t
TypeID::GetAttributeN() const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return type.attributes.size();
}

TypeID::AttributeInformation
TypeID::GetAttribute(size_t ith) const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return type.attributes[ith];
}

/*Constructor functions*/
Callback<ObjectBase*>
TypeID::GetConstructor() const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return type.constructor;
}

bool
TypeID::HasConstructor() const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return !type.constructor.IsNull();
}

/*Parent functions*/
TypeID
TypeID::GetParent() const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return TypeID(type.parent);
}

bool
TypeID::HasParent() const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return type.parent != 0;
}

/*Size functions*/
size_t
TypeID::GetSize() const
{
	const TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	return type.size;
}

TypeID&
TypeID::AddSize(size_t size)
{
	TypeInfo& type = TypeInfoManager::GetInstance()->GetTypeInfo(m_tid);
	type.size = size;
	return *this;
}

}