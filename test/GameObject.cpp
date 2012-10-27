#include "GameObject.h"
#include "Component.h"

#include <iostream>
using namespace std;

GameObject::GameObject()
	: _name("GameObject")
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