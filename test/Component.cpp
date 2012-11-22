#include "Component.h"
#include "GameObject.h"
#include "../source/LawlProps.h"
using namespace LawlProps;

Component::Component()
	: _parent(0)
{}

Component::~Component()
{}

void Component::Register()
{
	Component temp;
	TypeMeta<Component>::AddMember("Parent", temp, temp._parent);
}

void Component::Print(int depth)
{
	// Print out the reflection data
}