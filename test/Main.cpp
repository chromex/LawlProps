#include <iostream>
using namespace std;

#include "../source/LawlProps.h"
#include "GameObject.h"

int main(int argc, char** argv)
{
	cout << "Testing LawlProps" << endl;
	cout << "-----------------" << endl;

	const char* testJSON = "{\"Class\": \"GameObject\", \"_name\": \"Root\", \"_components\": [], \"_children\": [], }";
	cout << "Test load file:" << endl;
	cout << testJSON << endl;

	LawlProps::PropertyObjectVec objs = LawlProps::Load(testJSON);
	for(LawlProps::PropertyObjectVec::iterator obj = objs.begin(); obj != objs.end(); ++obj)
	{
		GameObject* go = (GameObject*)(*obj);
		go->Print(0);
	}

	cout << "-----------------" << endl;
	cout << "Done" << endl;
}