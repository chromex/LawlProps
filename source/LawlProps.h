#ifndef LAWLPROPS_H
#define LAWLPROPS_H

#include <vector>

namespace LawlProps
{
	class PropertyObject
	{
		// pointer to data definition?
		// virtual getter and setters?
	};

	typedef std::vector<PropertyObject*> PropertyObjectVec;
	void Load(const char* data, PropertyObjectVec& objs);
}

#endif