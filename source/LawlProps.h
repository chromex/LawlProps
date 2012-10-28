#ifndef LAWLPROPS_H
#define LAWLPROPS_H

#include <vector>

#include "LawlProps_Factory.h"

namespace LawlProps
{
	class PropertyObject
	{
	public:

	protected:


	private:

		// pointer to data definition?
		// virtual getter and setters?
	};

	typedef std::vector<PropertyObject*> PropertyObjectVec;
	void Load(const char* data, PropertyObjectVec& objs);
}

#endif