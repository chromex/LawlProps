#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	void Print(int depth);

	// Called by the game objects adder
	void SetParent(GameObject* parent) {_parent = parent;}

private:
	GameObject* _parent;
};

#endif