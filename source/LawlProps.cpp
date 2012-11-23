#include "LawlProps.h"

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

	void* Instantiate(const std::string& className)
	{
		TypeMetaBase* base = TypeStore::Instance().GetTypeMetaInstance(className);
		if(0 == base)
		{
			return 0;
		}
		return base->Instantiate();
	}

	void Parse(LJObject& object, void* ptr)
	{
		if(object.end() == object.find("Class") || !object["Class"].IsString())
		{
			return;
		}

		TypeMetaBase* base = TypeStore::Instance().GetTypeMetaInstance(object["Class"].string());
		const PropertyMap& propertyMap = base->GetPropertyMap();

		for(LJObject::iterator property = object.begin(); property != object.end(); ++property)
		{
			if(property->first == "Class")
				continue;

			PropertyMap::const_iterator entry = propertyMap.find(property->first);
			if(propertyMap.end() == entry)
			{
				// Unmatched property
				continue;
			}

			void* propPtr = reinterpret_cast<void*>((size_t)ptr + entry->second.offset);
			size_t propTypeID = entry->second.type->GetID();

			if(property->second.IsNumber())
			{
				if(TypeMeta<int>::ID() == propTypeID)
				{
					*((int*)propPtr) = (int)property->second.number();
				}
				else if(TypeMeta<unsigned int>::ID() == propTypeID)
				{
					*((unsigned int*)propPtr) = (unsigned int)property->second.number();
				}
				else if(TypeMeta<float>::ID() == propTypeID)
				{
					*((float*)propPtr) = (float)property->second.number();
				}
				else if(TypeMeta<double>::ID() == propTypeID)
				{
					*((double*)propPtr) = (double)property->second.number();
				}
				else
				{
					// Types don't match
				}
			}
			else if(property->second.IsBoolean())
			{
				if(TypeMeta<bool>::ID() == propTypeID)
				{
					*((bool*)propPtr) = property->second.boolean();
				}
				else
				{
					// Types don't match
				}
			}
			else if(property->second.IsString())
			{
				if(TypeMeta<std::string>::ID() == propTypeID)
				{
					*((std::string*)propPtr) = property->second.string();
				}
				else
				{
					// Types don't match
				}
			}
			else if(property->second.IsObject())
			{
				Parse(property->second.object(), propPtr);
			}
		}
	}

	void Export(LJObject& object, TypeMetaBase* meta, void* ptr)
	{
		object["Class"] = meta->GetName();

		const PropertyMap& propertyMap = meta->GetPropertyMap();
		for(PropertyMap::const_iterator entry = propertyMap.begin(); entry != propertyMap.end(); ++entry)
		{
			void* propPtr = reinterpret_cast<void*>((size_t)ptr + entry->second.offset);
			size_t propID = entry->second.type->GetID();

			if(TypeMeta<int>::ID() == propID)
			{
				object[entry->first] = (LJNumber)(*((int*)propPtr));
			}
			else if(TypeMeta<unsigned int>::ID() == propID)
			{
				object[entry->first] = (LJNumber)(*((unsigned int*)propPtr));
			}
			else if(TypeMeta<float>::ID() == propID)
			{
				object[entry->first] = (LJNumber)(*((float*)propPtr));
			}
			else if(TypeMeta<double>::ID() == propID)
			{
				object[entry->first] = (LJNumber)(*((double*)propPtr));
			}
			else if(TypeMeta<std::string>::ID() == propID)
			{
				object[entry->first] = (LJString)(*((std::string*)propPtr));
			}
			else if(TypeMeta<bool>::ID() == propID)
			{
				object[entry->first] = (bool)(*((bool*)propPtr));
			}
			else
			{
				object[entry->first] = LJObject();
				Export(object[entry->first].object(), entry->second.type, propPtr);
			}
		}
	}
}