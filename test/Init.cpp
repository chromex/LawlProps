#include "Init.h"

#include "GameObject.h"
#include "Component.h"

void InitFactory()
{
	GameObject::Register();
}