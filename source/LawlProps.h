#ifndef LAWLPROPS_H
#define LAWLPROPS_H

#include <vector>
#include <LawlJSON.h>
#include <iostream>

using namespace LJ_NAMESPACE;

#include "LawlProps_Factory.h"

namespace LawlProps
{
	template< typename T >
	T* Parse(LJValue& json)
	{
		if(json.IsObject())
		{
			LJObject& obj = json.object();

			if(obj.end() == obj.find("Class"))
			{
				// Throw exception...
				std::cout << "Object encountered with no class specified" << std::endl;
				return 0;
			}

			if(!Factory::HasType(obj["Class"].string()))
			{
				// Throw exception
				std::cout << "Class not specified in factory" << std::endl;
				return 0;
			}

			// Get the class data
			size_t typeID = Factory::TypeID(obj["Class"].string());

			// Create an instance of the object
			T* newObj = new T;

			for(LJObject::const_iterator prop = obj.begin(); prop != obj.end(); ++prop)
			{
				if(prop->first == "Class")
					continue;

				if(prop->second.IsNumber())
				{
					if(Factory::HasClassMember<int>(typeID, prop->first))
					{
						Factory::Set(newObj, prop->first.c_str(), (int)prop->second.number());
					}
					else if(Factory::HasClassMember<unsigned int>(typeID, prop->first))
					{
						Factory::Set(newObj, prop->first.c_str(), (unsigned int)prop->second.number());
					}
					else if(Factory::HasClassMember<float>(typeID, prop->first))
					{
						Factory::Set(newObj, prop->first.c_str(), (float)prop->second.number());
					}
					else if(Factory::HasClassMember<double>(typeID, prop->first))
					{
						Factory::Set(newObj, prop->first.c_str(), (double)prop->second.number());
					}
					else
					{
						// Throw exception...
					}
				}
				else if(prop->second.IsBoolean())
				{
					if(Factory::HasClassMember<bool>(typeID, prop->first))
					{
						Factory::Set(newObj, prop->first.c_str(), prop->second.boolean());
					}
					else
					{
						// Throw exception...
					}
				}
				else if(prop->second.IsString())
				{
					if(Factory::HasClassMember<std::string>(typeID, prop->first))
					{
						Factory::Set(newObj, prop->first.c_str(), prop->second.string());
					}
					else
					{
						// Throw exception...
					}
				}
				else if(prop->second.IsObject())
				{

				}
			}

			return newObj;
		}

		return 0;
	}

	template< typename T >
	T* Load(const char* data)
	{
		if(!Factory::HasType<T>())
		{
			std::cout << "Type to load not specified in factory" << std::endl;
			return 0;
		}

		LJValue results;
		ParseJSON(data, results);
		return Parse<T>(results);
	}
}

#endif