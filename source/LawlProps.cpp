#include "LawlProps.h"
#include <LawlJSON.h>

#include <iostream>

using namespace std;
using namespace LJ_NAMESPACE;

namespace LawlProps
{
	void Parse(LJValue& json, PropertyObjectVec& objs);

	void Load(const char* data, PropertyObjectVec& objs)
	{
		LJValue results;
		ParseJSON(data, results);
		Parse(results, objs);
	}

	void Parse(LJValue& json, PropertyObjectVec& objs)
	{
		if(json.IsObject())
		{
			LJObject& obj = json.object();

			if(obj.end() == obj.find("Class"))
			{
				// Should throw an exception...
				cout << "Object encountered with no class specified" << endl;
				return;
			}

			// Verify the class supports the children

			// 
		}
	}
}