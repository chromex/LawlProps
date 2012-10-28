#include <iostream>
using namespace std;

#include "../source/LawlProps.h"
#include "GameObject.h"
#include "Init.h"

int main(int argc, char** argv)
{
	cout << "--> Testing LawlProps <--" << endl << endl;

	InitFactory();

	cout << "+ Factory Initialized +" << endl;

	cout << LawlProps::Factory::ToString();

	const char* testJSON = "{\"Class\": \"GameObject\", \"Name\": \"Root\", \"ID\": 54 }";
	cout << "+ Test Data +" << endl;
	cout << testJSON << endl;

	LawlProps::PropertyObjectVec objs;
	LawlProps::Load(testJSON, objs);
	for(LawlProps::PropertyObjectVec::iterator obj = objs.begin(); obj != objs.end(); ++obj)
	{
		GameObject* go = (GameObject*)(*obj);
		go->Print(0);
	}

	cout << endl << "--> Test Complete <--" << endl;
}