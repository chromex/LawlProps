#include "Init.h"

#include "GameObject.h"
#include "Component.h"
#include "Vec3.h"

void InitFactory()
{
	GameObject::Register();
	Component::Register();

	{
		vec3 temp;
		LawlProps::AddClassMember(temp, "X", temp.x);
		LawlProps::AddClassMember(temp, "Y", temp.y);
		LawlProps::AddClassMember(temp, "Z", temp.z);
	}
}