#include "GameObject.h"
#include "Component.h"
#include "../source/LawlProps.h"
#include <stddef.h>

#include <iostream>
using namespace std;

GameObject::GameObject()
	: _name("GameObject")
	, _id(-1)
{}

GameObject::~GameObject()
{
	// Destroy components
	for(ComponentVec::iterator ptr = _components.begin(); ptr != _components.end(); ++ptr)
	{
		delete *ptr;
	}

	// Destory children
	for(GameObjectVec::iterator ptr = _children.begin(); ptr != _children.end(); ++ptr)
	{
		delete *ptr;
	}
}

void GameObject::Register()
{
	GameObject temp;

	LawlProps::AddClassMember(temp, "Name", temp._name);
	LawlProps::AddClassMember(temp, "ID", temp._id);
}

void GameObject::Print(int depth)
{
	for(int idx = 0; idx < depth; ++idx)
		cout << "  ";

	cout << _name << " (" << endl;

	for(ComponentVec::iterator ptr = _components.begin(); ptr != _components.end(); ++ptr)
	{
		(*ptr)->Print(depth+1);
	}

	for(GameObjectVec::iterator ptr = _children.begin(); ptr != _children.end(); ++ptr)
	{
		(*ptr)->Print(depth+1);
	}

	for(int idx = 0; idx < depth; ++idx)
		cout << "  ";

	cout << ")" << endl;
}