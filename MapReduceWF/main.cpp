#include "MapReduceWF.h"
#include "FileIO.h"
#include <vector>
#include <string>
#include <iostream>


// Print Banner at the top of the Program
void printBanner();

// Print Help Message that displays the correct arguments to provide
void printHelp();

/// Convert Char* Arguments to Strings within a Vector 
void convertArgsToStrings(int numberOfArguments, char* argumentArray[], std::vector<std::string>& argumentVector);

int main(int argc, char* argv[])
{
	printBanner();
	std::vector<std::string> argumentVector;
	convertArgsToStrings(argc, argv, argumentVector);

	std::vector<std::string>::iterator argVecIT = find(argumentVector.begin(), argumentVector.end(), "-help");
	if (argVecIT != argumentVector.end())
	{
		return -1;
	}
	else if (argumentVector.size() == 1)
	{
	
	}

	else
	{
		printHelp();
		return -1;
	}

	// Pass in Directories into construction of Map Reducer
	MapReducer mapReducer(argumentVector.at(1), argumentVector.at(2), argumentVector.at(3), argumentVector.at(4), argumentVector.at(5));
	std::string outputFileName = ""; //TODO
	if (mapReducer.reduce(outputFileName))
	{
		std::cout << "Successfully Reduced Input Directory into the file: "<< outputFileName << "!" << std::endl;
	}
	else
	{
		std::cout << "Failed to Reduce Input Directory!" << std::endl;
	}
	
	return 0;
}

void printBanner()
{
	// Print Banner to Screen
	std::cout << "------------------------- Phase 2 - Map Reducer ------------------------" << std::endl;
#ifdef PRINT_DEBUG	
	std::cout << "C++ Version is " << __cplusplus << std::endl;
#endif
	std::cout << "This program is a standalone command-line program that can run a word count MapReduce Work Flow. \n" <<
		"A set of text files are stored and will ultimately produce a single output file that contains \n" <<
		"a list of words and their associated counts in the originating input files." << std::endl;
	std::cout << "-----------------------------------------------------------------------" << std::endl;
}

void printHelp()
{
	std::cout << "The Program should be provided 3-5 Arguments in the following Order: (Paths should be relative) " << std::endl;
	std::cout << 
		"     a) Path to the Map DLL \n" <<
		"     b) Path to the Reduce DLL \n" << 
		"     c) Path to the Directory that holds input files.\n" <<
		"     d) Path to the Directory to hold output file. (Optional) \n" <<
		"     e) Temporary directory to hold intermediate output files. (Optional) \n " << std::endl;
}

void convertArgsToStrings(int numberOfArguments, char* argumentArray[], std::vector<std::string>& argumentVector)
{
	for (int i = 0; i < numberOfArguments; ++i)
	{
		argumentVector.push_back(std::string(argumentArray[i]));
	}
}