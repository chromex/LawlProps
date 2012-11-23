#include <iostream>
using namespace std;

#include "../source/LawlProps.h"
using namespace LawlProps;

#include "GameObject.h"
#include "Component.h"
#include "Vec3.h"

#define REDCOLOR "\033[1;31m"
#define GREENCOLOR "\033[1;32m"
#define CLEARCOLOR "\033[0m"

int main(int argc, char** argv)
{
	cout << "[" << REDCOLOR << "Testing LawlProps" << CLEARCOLOR << "]" << endl;

	GameObject::Register();
	Component::Register();
	{
		vec3 temp;
		TypeMeta<vec3>::AddMember("X", temp, temp.x);
		TypeMeta<vec3>::AddMember("Y", temp, temp.y);
		TypeMeta<vec3>::AddMember("Z", temp, temp.z);
	}

	cout << "Types Loaded..." << endl;

	cout << "[" << GREENCOLOR << "Type Definitions" << CLEARCOLOR << "]" << endl;

	cout << TypeStore::Instance().ToString();

	cout << "[" << GREENCOLOR << "Test Load" << CLEARCOLOR << "]" << endl;

	const char* testJSON = "{\"Class\":\"GameObject\",\"Name\":\"Root\",\"Enabled\":true,\"ID\":54,\"Position\":{\"Class\":\"vec3\",\"X\":1,\"Y\":2,\"Z\":3}}";
	cout << testJSON << endl;

	GameObject* root = Load<GameObject>(testJSON);
	root->Print(0);

	cout << "[" << GREENCOLOR << "Test Save" << CLEARCOLOR << "]" << endl;

	cout << Save(root) << endl;

	cout << "[" << REDCOLOR << "Test Complete" << CLEARCOLOR << "]" << endl;
}