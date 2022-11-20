// MapReduceWF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// mapReduceCMake.cpp : Defines the entry point for the application.


#include "MapReduceWF.h"
#include <iostream>
#include <Windows.h>
// PUBLIC METHODS


MapReducer::MapReducer(std::string mapDll, std::string reduceDll, std::string inputDir, std::string outputDir, std::string middleDir) :
	mapDllLocation_(mapDll),
	reduceDllLocation_(reduceDll),
	inputDirectory_(inputDir),
	outputDirectory_(outputDir),
	intermediateDirectory_(middleDir),
	mapSorter(folderNameForMapOutput, folderNameForSorterOutput)
{}


bool MapReducer::reduce(std::string& outputFileName)
{
	return doReduce(outputFileName);
}

std::string MapReducer::getInputDir()
{
	return inputDirectory_;
}

std::string MapReducer::getOutputDir()
{
	return outputDirectory_;
}

std::string MapReducer::getIntermediateDir()
{
	return intermediateDirectory_;
}

void MapReducer::setInputDir(std::string in)
{
	inputDirectory_ = in;
}

void MapReducer::setOutputDir(std::string out)
{
	outputDirectory_ = out;
}

void MapReducer::setIntermediateDir(std::string middle)
{
	intermediateDirectory_ = middle;
}

bool MapReducer::setDefaultDirectory(std::string defaultDir)
{
	bool results = fileManager.validDirectory("..\\" + defaultDir + "Dir_Default");

	// check if a default directory exists
	if (results == false)
	{
		results = fileManager.createDirectory("..\\", defaultDir + "Dir_Default"); //temp directory in working folder
		if (results == false)
		{
			std::cout << "Terminating: Failed to create default " + defaultDir + " directory" << std::endl;
			return results;
		}
		std::cout << "INFO: New output directory ..\\" + defaultDir + "Dir_Default created successfully!" << std::endl;
	}
	return results;
	//setOutputDir("..\\OutputDir_Default");
}


// PRIVATE METHODS 
bool MapReducer::validateDirectories()
{
	//Check Input Directory
	bool results = fileManager.validDirectory(inputDirectory_);
	// SROA: Determine if Directory Existing if it does and its not valid
	if (results == false)
	{
		std::cout << "ERROR: You must specify a valid input Directory" << std::endl;
		return results;
	}

	results = results && fileManager.validDirectory(outputDirectory_);
	if (results == false)
	{
		results = setDefaultDirectory("output");
		if (results == false) { return results; }
		setOutputDir("..\\outputDir_Default");
		std::cout << "INFO: Output directory set to default: ..\\outputDir_Default" << std::endl;
	}

	//Check Intermediate Directory
	results = results && fileManager.validDirectory(intermediateDirectory_);
	if (results == false)
	{
		results = setDefaultDirectory("middle");
		if (results == false) { return results; }
		setIntermediateDir("..\\middleDir_Default");
		std::cout << "INFO: intermediate directory set to default: ..\\middleDir_Default" << std::endl;
	}

	if (results == true)
	{
		results = results && fileManager.createDirectory(intermediateDirectory_, folderNameForMapOutput);
		results = results && fileManager.createDirectory(intermediateDirectory_, folderNameForSorterOutput);
		if (results == false)
		{
			std::cout << "Couldn't create MapOutput or SorterOutput, Terminating" << std::endl;
			return results;
		}
	}

	//Clear Output Directory
	if (results == true)
	{
		std::vector<std::string> fileList;
		fileManager.getListOfTextFiles(outputDirectory_, fileList);
		for (size_t count = 0; count < fileList.size(); count++)
		{
			// Stuff exist in output directory...... kill it
			std::string totalFilePath = outputDirectory_ + "\\" + fileList.at(count);
			if (!fileManager.deleteFile(totalFilePath))
			{
				std::cout << "Unable to Delete (" << totalFilePath << ") in the OUtput Directory" << std::endl;
				results = false;
			}
		}
	}
	return results;
}
bool MapReducer::MapStepDLL(std::string& dllLocaiton, const std::string& outputMapDirectory)
{
	std::vector<std::string> fileList;
	MapInterface* mapIF = NULL;
	HINSTANCE hdllMap = NULL;
	pvFunctv CreateMap;
	
	bool results = true;

	hdllMap = LoadLibraryA(dllLocaiton.c_str());

	if (hdllMap != NULL)
	{
		CreateMap = (pvFunctv)(GetProcAddress(hdllMap, "CreateMapClassInstance"));
		if (CreateMap == nullptr)
		{
			std::cout << "Error: Did not load CreateMapClassInstance correctly." << std::endl;
			results = false;
		}
		else
		{
			mapIF = (static_cast<MapInterface*> (CreateMap()));	// get pointer to object
			if (mapIF == nullptr)
			{
				std::cout << "Error: Map Interface Not implemented correctly " << std::endl;
				results = false;
			}
			else
			{
				std::cout << "Prior Set Parameters" << std::endl;
				mapIF->setParameters(outputMapDirectory, bufferSize);
				std::cout << "Post Set Parameter" << std::endl;

				// For the input Directory read the list of files 
				fileManager.getListOfTextFiles(inputDirectory_, fileList);

				// Input Processing and Map Call for Each file 
				for (size_t fileCount = 0; fileCount < fileList.size(); fileCount++)
				{
					std::vector<std::string> lines;
					// Read Each File Into a Vector of Strings 
					if (fileManager.readFileIntoVector(inputDirectory_, fileList.at(fileCount), lines))
					{
						// Call the Map Function for each Line
						for (size_t fileLine = 0; fileLine < lines.size(); fileLine++)
						{
							uint32_t count = 0;
							//Map Function --> Map
							mapIF->createMap(fileList.at(fileCount), lines.at(fileLine));
							std::cout << "CreateMapCall " << fileList.at(fileCount) << " " << count << std::endl;
						}

						//Map Function --> Export
						mapIF->flushMap(fileList.at(fileCount));
						lines.resize(0);
					}
				}
			}
		}
	}
	else
	{
		std::cout << "Error: Map Library load failed!" << std::endl;
		results = false;
	}

	return results;
}

/*doReduce function within the class ensures a directory exists, then proceeds read files in the input directory, it takes files from the input directory and writes
the file content into a vector named lines, calling the map function, it takes each line processes it by removing an punctuation and emits a value 1 for each key.
after it completes it flushes and empties the cache. Now calling the sorter class, it shuffles each value to its respective keyword, such as ([for], [1,1,1,1])
Later the reducer is called to collect each instance for a ketword and sum it to reduce the amount of memory used by each value. after completion it exports the data
to the outputDirectory*/
bool MapReducer::doReduce(std::string& outputFileName)
{	
	if (validateDirectories())
	{
		std::shared_ptr<MapInterface> mapIF = nullptr;
		std::vector<std::string> fileList;
		std::string sortedFileName;
		std::string outputMapDirectory = intermediateDirectory_ + "\\" + folderNameForMapOutput;
		std::string outputSortDirectory = intermediateDirectory_ + "\\" + folderNameForSorterOutput;
		if (!MapStepDLL(mapDllLocation_, outputMapDirectory)) return false;
		

		if (!mapSorter.sortMappedFiles(outputMapDirectory, outputSortDirectory, sortedFileName))
		{
			std::cout << "ERROR: Unable to Sort Mapped Files Output" << std::endl;
			return false;
		}
		
		if (!ReduceStepDLL(reduceDllLocation_, outputSortDirectory, sortedFileName, outputFileName)) return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool MapReducer::ReduceStepDLL(const std::string& dllLocaiton, const std::string& outputSortDirectory, const std::string& sortedFileName, std::string& outputFileName)
{
	bool result = true;
	HINSTANCE hdllReduce = NULL;
	ReduceInterface* piReduce = NULL;
	pvFunctv CreateReduce;
	hdllReduce = LoadLibraryA(dllLocaiton.c_str());
	if (hdllReduce != NULL)
	{
		CreateReduce = (pvFunctv)(GetProcAddress(hdllReduce, "CreateReduceClassInstance"));
		if (CreateReduce != nullptr)
		{
			piReduce = static_cast<ReduceInterface*> (CreateReduce());	// get pointer to object

			if (piReduce != NULL)
			{
				piReduce->setParameters(outputDirectory_);
				if (!piReduce->reduceFile(outputSortDirectory, sortedFileName, outputFileName)) // Pulls File and puts entire line into Vect
				{
					std::cout << "ERROR: Unable to import Sorted Data into Reducer" << std::endl;
					result = false;
				}
			}
			else
			{
				std::cout << "Error: Could not create ReduceInterface Class." << std::endl;
				result = false;
			}
		}
		else
		{
			std::cout << "Error: Did not load CreateReduceClassInstance correctly." << std::endl;
			result = false;
		}
	}
	else
	{
		std::cout << "Error: Reduce Library load failed!" << std::endl;
		result = false;
	}
	return result;
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
