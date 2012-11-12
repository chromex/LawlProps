#include "Component.h"
#include "GameObject.h"
#include "../source/LawlProps.h"

Component::Component()
	: _parent(0)
{}

Component::~Component()
{}

void Component::Register()
{
	Component temp;

	LawlProps::AddClassMember(temp, "Parent", temp._parent);
}

void Component::Print(int depth)
{
	// Print out the reflection data
}