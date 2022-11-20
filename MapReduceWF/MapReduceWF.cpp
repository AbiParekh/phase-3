// MapReduceWF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// mapReduceCMake.cpp : Defines the entry point for the application.


#include "MapReduceWF.h"
#include <iostream>
#include <Windows.h>
// PUBLIC METHODS


typedef void* (*pvFunctv)();


MapReducer::MapReducer(std::string configFileLocation) :
	configurationFileLocation_(configFileLocation), 
	mapSorter("", "") {}


bool MapReducer::reduce(std::string& outputFileName)
{
	return doReduce(outputFileName);
}


/*doReduce function within the class ensures a directory exists, then proceeds read files in the input directory, it takes files from the input directory and writes
the file content into a vector named lines, calling the map function, it takes each line processes it by removing an punctuation and emits a value 1 for each key.
after it completes it flushes and empties the cache. Now calling the sorter class, it shuffles each value to its respective keyword, such as ([for], [1,1,1,1])
Later the reducer is called to collect each instance for a ketword and sum it to reduce the amount of memory used by each value. after completion it exports the data
to the outputDirectory*/
bool MapReducer::doReduce(std::string& outputFileName)
{	
	if (mapReduceConfig.parseConfigurationFile(configurationFileLocation_))
	{
		std::shared_ptr<MapInterface> mapIF = nullptr;
		std::vector<std::string> fileList;
		std::string sortedFileName;
		std::string outputMapDirectory = mapReduceConfig.getIntermediateDir() + "\\" + mapReduceConfig.getMapTempOutputFolder();
		std::string outputReduceDirectory = mapReduceConfig.getIntermediateDir() + "\\" + mapReduceConfig.getReduceTempOutputFolder();
		std::string mapDLLLocation = mapReduceConfig.getMapDllLocation();
		std::string reduceDLLLocation = mapReduceConfig.getReduceDllLocation();
		if (!MapStepDLL(mapDLLLocation, outputMapDirectory)) return false;
		

		if (!mapSorter.sortMappedFiles(outputMapDirectory, outputReduceDirectory, sortedFileName))
		{
			std::cout << "ERROR: Unable to Sort Mapped Files Output" << std::endl;
			return false;
		}
		
		if (!ReduceStepDLL(reduceDLLLocation, outputReduceDirectory, sortedFileName, outputFileName)) return false;
	}
	else
	{
		return false;
	}
	return true;
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
				fileManager.getListOfTextFiles(mapReduceConfig.getInputDir(), fileList);

				// Input Processing and Map Call for Each file 
				for (size_t fileCount = 0; fileCount < fileList.size(); fileCount++)
				{
					std::vector<std::string> lines;
					// Read Each File Into a Vector of Strings 
					if (fileManager.readFileIntoVector(mapReduceConfig.getInputDir(), fileList.at(fileCount), lines))
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
				piReduce->setParameters(mapReduceConfig.getOutputDir());
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

