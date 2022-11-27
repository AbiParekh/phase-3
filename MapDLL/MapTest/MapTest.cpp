// MathTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#include <MapInterface.h>

typedef void* (*pvFunctv)();


int main()
{
	HINSTANCE hdll = NULL;
	InterfaceMap* myMap = NULL;
	pvFunctv CreateMap;
	hdll = LoadLibrary(TEXT("..//MapLibrary//x64//Debug//MapLibrary.dll"));

	if (hdll != NULL) {
		CreateMap = (pvFunctv) (GetProcAddress(hdll, "CreateMapClassInstance"));
		if (CreateMap != nullptr)
		{
			myMap = static_cast<InterfaceMap*> (CreateMap());	// get pointer to object

			if (myMap != NULL)
			{
				//myMap->ProofDLLWorks();
				myMap->setParameters("..\\middleDir", 3000, 3);
				std::cout << myMap->printParameters("HI");
				myMap->createMap("FakeFile.txt", "apple, banana, carrot, dates, eggplant, figs, grapes, honey, icecreame, jackfruit, kiwi, lemon, melon, nectarine, orange," 
					"pomegranite, quiche, raisin, sunflower, tangerine, udon, vanilla, watermelon, xigua, yogurt, zucchini 'apostrophe, o'clock");
				myMap->flushMap("FakeFile.txt");

			}
			else
			{
				std::cout << "Could not create InterfaceMap Class." << std::endl;
			}
		}
		else
		{
			std::cout << "Did not load CreateFooClassInstance correctly." << std::endl;
		}

		FreeLibrary(hdll);
	}
	else 
	{
		std::cout << "Library load failed!" << std::endl;
	}

	std::cout << "Press any key to quit";
	std::cin.get();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
