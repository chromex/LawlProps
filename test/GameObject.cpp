#include "GameObject.h"
#include "Component.h"

#include "../source/LawlProps.h"
using namespace LawlProps;

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
	TypeMeta<GameObject>::AddMember("Enabled", temp, temp._enabled);
	TypeMeta<GameObject>::AddMember("Name", temp, temp._name);
	TypeMeta<GameObject>::AddMember("ID", temp, temp._id);
	TypeMeta<GameObject>::AddMember("Position", temp, temp._pos);
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