#include "Component.h"
#include "GameObject.h"

Component::Component()
	: _parent(0)
{}

Component::~Component()
{}

void Component::Print(int depth)
{
	// Print out the reflection data
}