#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>

class Component;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Print(int depth);

private:
	typedef std::vector<GameObject*> GameObjectVec;
	typedef std::vector<Component*> ComponentVec;

	std::string _name;
	GameObjectVec _children;
	ComponentVec _components;
};

#endif