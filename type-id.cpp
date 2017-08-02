#include "type-id.h"
#include "singleton.h"
#include "console-print.h"

#include <string>
#include <vector>
#include <map>

namespace eventsim
{

class TypeInfoManager : public Singleton<TypeInfoManager>
{

public:
	uint16_t AllocateType(const char* classname);
	

private:
	TypeInfoManager();

	/*Type information*/
	struct TypeInfo
	{
		std::string									name;
		uint16_t									parent;
		size_t										size;
		std::vector<TypeID::AttributeInformation>	attributes;
	};

	typedef std::vector<struct TypeInfo>	  TypeInfo_t;
	typedef std::map<std::string, uint16_t>   NameMap_t;

	/*Runtime container of the type information*/
	TypeInfo_t m_typeInfos;
	/*By name index*/
	NameMap_t  m_nameMap; 
};



uint16_t TypeInfoManager::AllocateType(const char* classname)
{
	NameMap_t::const_iterator it = m_nameMap.find(classname);
	if (it != m_nameMap.end())
	{
		OUT_MSG("Duplicate type error");
		exit(-1);
	}

	TypeInfo ntype;
	ntype.parent = -1;
}


TypeID::TypeID(const char* classname)
{
	
}

}