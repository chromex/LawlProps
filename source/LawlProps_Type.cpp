#include "LawlProps_Type.h"

#include <cxxabi.h>
#include <stdlib.h>
#include <sstream>

namespace LawlProps
{
	TypeStore& TypeStore::Instance()
	{
		static TypeStore store;
		return store;
	}

	// A gcc specific solution
	// http://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
	std::string TypeStore::Demangle(const char* name)
	{
		int status = -4;
		char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
		const char* const demangled_name = (status==0)?res:name;
		std::string ret_val(demangled_name);
		free(res);
		return ret_val;
	}

	size_t TypeStore::UniqueID(const std::string& name)
	{
		typedef std::map<std::string, size_t> NameMap;
		static NameMap nameMap;
		static size_t nextID = 0;

		if(nameMap.end() == nameMap.find(name))
		{
			nameMap[name] = nextID++;
		}

		return nameMap[name];
	}

	TypeMetaBase* TypeStore::GetTypeMetaInstance(const std::string& name)
	{
		if(_nameMap.end() == _nameMap.find(name))
		{
			return 0;
		}
		else
		{
			return _nameMap[name];
		}
	}

	void TypeStore::SetTypeMetaInstance(const std::string& name, TypeMetaBase* ptr)
	{
		_nameMap[name] = ptr;
	}

	std::string TypeStore::ToString() const
	{
		std::stringstream ss;

		for(NameMap::const_iterator entry = _nameMap.begin(); entry != _nameMap.end(); ++entry)
		{
			ss << entry->first << " " << entry->second->GetID() << std::endl;

			for(PropertyMap::const_iterator property = entry->second->GetPropertyMap().begin();
				property != entry->second->GetPropertyMap().end(); ++property)
			{
				ss << "-- " << property->second.type->GetName() << " " << property->first << std::endl;
			}
		}

		return ss.str();
	}
}