#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../source/LawlProps.h"
#include "Vec3.h"

#include <vector>
#include <string>

class Component;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	static void Register();

	void Print(int depth);

private:
	typedef std::vector<GameObject*> GameObjectVec;
	typedef std::vector<Component*> ComponentVec;

	int _id;
	std::string _name;
	GameObjectVec _children;
	ComponentVec _components;
	vec3 _pos;
	vec3* _ptrPos;
	bool _enabled;
};

#endif