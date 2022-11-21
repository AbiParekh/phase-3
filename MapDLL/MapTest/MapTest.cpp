// MathTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#include "MapInterface.h"

typedef void* (*pvFunctv)();


int main()
{
	HINSTANCE hdll = NULL;
	InterfaceMap* piFoo = NULL;
	pvFunctv CreateFoo;
	hdll = LoadLibrary(TEXT("MapLibrary.dll"));

	if (hdll != NULL) {
		CreateFoo = (pvFunctv) (GetProcAddress(hdll, "CreateMapClassInstance"));
		if (CreateFoo != nullptr)
		{
			piFoo = static_cast<InterfaceMap*> (CreateFoo());	// get pointer to object

			if (piFoo != NULL)
			{
				piFoo->ProofDLLWorks();
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
