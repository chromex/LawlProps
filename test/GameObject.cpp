#include "GameObject.h"
#include "Component.h"
#include "../source/LawlProps.h"
#include <stddef.h>

#include <iostream>
using namespace std;

GameObject::GameObject()
	: _name("GameObject")
	, _id(-1)
	, _pos(0.f)
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

	LawlProps::AddClassMember(temp, "Enabled", temp._enabled);
	LawlProps::AddClassMember(temp, "Name", temp._name);
	LawlProps::AddClassMember(temp, "ID", temp._id);
	LawlProps::AddClassMember(temp, "Position", temp._pos);
}

void PrintTabs(int num)
{
	for(int idx = 0; idx < num; ++idx)
		cout << "  ";
}

void GameObject::Print(int depth)
{
	PrintTabs(depth); cout << _name << " (" << endl;

	PrintTabs(depth+1); cout << "Enabled: " << _enabled << endl;
	PrintTabs(depth+1); cout << "ID: " << _id << endl;
	PrintTabs(depth+1); cout << "Name: " << _name << endl;
	//PrintTabs(depth+1); cout << "Position: " << _pos << endl;

	for(ComponentVec::iterator ptr = _components.begin(); ptr != _components.end(); ++ptr)
	{
		(*ptr)->Print(depth+1);
	}

	for(GameObjectVec::iterator ptr = _children.begin(); ptr != _children.end(); ++ptr)
	{
		(*ptr)->Print(depth+1);
	}

	PrintTabs(depth); cout << ")" << endl;
}