#include "LawlProps_Factory.h"
#include <cxxabi.h>
#include <stdlib.h>
#include <exception>
#include <sstream>

namespace LawlProps
{
	Factory::Factory()
	{}

	Factory::~Factory()
	{
		// Clear out the data
	}

	Factory& Factory::Inst()
	{
		static Factory inst;
		return inst;
	}

	std::string Factory::ToString()
	{
		return Inst()._ToString();
	}

	FactoryClass* Factory::AddOrCreateClass(const std::string& className)
	{
		size_t type = TypeID(className);

		if(_classMap.end() == _classMap.find(type))
		{
			_classMap[type] = new FactoryClass;
			_classMap[type]->name = className;
		}

		return _classMap[type];
	}

	std::string Factory::_ToString() const
	{
		std::stringstream ss;

		ss << "\n== Type IDs ==\n";
		for(TypeIDMap::const_iterator typeID = _typeIDs.begin(); typeID != _typeIDs.end(); ++typeID)
		{
			ss << typeID->first << " " << typeID->second << "\n";
		}

		ss << "\n== Classes ==\n";
		for(FactoryClassMap::const_iterator factoryClass = _classMap.begin(); factoryClass != _classMap.end(); ++factoryClass)
		{
			ss << (factoryClass->second)->ToString() << "\n";
		}

		return ss.str();
	}

	std::string FactoryClass::ToString() const
	{
		std::stringstream ss;
		ss << name << "\n";

		for(PropertyMap::const_iterator property = properties.begin(); property != properties.end(); ++property)
		{
			ss << "-- " << property->second.type << " " << property->first << "\n";
		}

		return ss.str();
	}

	void Factory::_AddClassMember(const std::string& className, const std::string& propertyName, size_t offset, size_t type)
	{
		FactoryClass* fc = AddOrCreateClass(className);
		if(fc->properties.end() != fc->properties.find(propertyName))
		{
			throw std::exception();
		}

		Property& p = fc->properties[propertyName];
		p.offset = offset;
		p.type = type;
	}

	size_t Factory::TypeID(const std::string& name)
	{
		static size_t currentID = 0;
		if(_typeIDs.end() == _typeIDs.find(name))
		{
			_typeIDs[name] = currentID;
			++currentID;
		}
		return _typeIDs[name];
	}

	size_t Factory::GetPropertyOffset(size_t classType, size_t memberType, const std::string& propertyName) const
	{
		FactoryClassMap::const_iterator fc = _classMap.find(classType);
		if(_classMap.end() == fc)
		{
			throw std::exception();
		}

		FactoryClass::PropertyMap::const_iterator prop = (*fc->second).properties.find(propertyName);
		if((*fc->second).properties.end() == prop)
		{
			throw std::exception();
		}

		if(prop->second.type != memberType)
		{
			throw std::exception();
		}

		return prop->second.offset;
	}

	std::string Factory::TypeName(size_t type)
	{
		for(TypeIDMap::const_iterator typeID = Inst()._typeIDs.begin(); typeID != Inst()._typeIDs.end(); ++typeID)
		{
			if(typeID->second == type)
			{
				return typeID->first;
			}
		}

		return "<unknown type>";
	}

	// A gcc specific solution :(
	// http://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
	std::string Factory::Demangle(const char* name)
	{
		int status = -4;
		char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
		const char* const demangled_name = (status==0)?res:name;
		std::string ret_val(demangled_name);
		free(res);
		return ret_val;
	}
}