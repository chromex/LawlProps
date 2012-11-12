#include <iostream>
using namespace std;

#include "../source/LawlProps.h"
#include "GameObject.h"
#include "Init.h"

int main(int argc, char** argv)
{
	cout << "--> Testing LawlProps <--" << endl;

	InitFactory();

	cout << LawlProps::Factory::ToString();

	const char* testJSON = "{\"Class\":\"GameObject\",\"Name\":\"Root\",\"Enabled\":true,\"ID\":54,\"Position\":{\"Class\":\"vec3\",\"X\":1,\"Y\":2,\"Z\":3}}";
	cout << endl << "--> Test Data <--" << endl;
	cout << testJSON << endl;

	GameObject* root = LawlProps::Load<GameObject>(testJSON);
	root->Print(0);

	cout << endl << endl << "--> Test Complete <--" << endl;
}