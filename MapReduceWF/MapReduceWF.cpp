// MapReduceWF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// mapReduceCMake.cpp : Defines the entry point for the application.


#include "MapReduceWF.h"
#include <iostream>
#include <Windows.h>



// PUBLIC METHODS

MapReducer::MapReducer(std::string configFileLocation) :
	configurationFileLocation_(configFileLocation) {}


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
{	/*
	if (mapReduceConfig.parseConfigurationFile(configurationFileLocation_))
	{
		std::shared_ptr<MapInterface> mapIF = nullptr;
		std::vector<std::string> fileList;
		std::string sortedFileName;
		std::string inputMapDirectory = mapReduceConfig.getInputDir();
		std::string outputMapDirectory = mapReduceConfig.getIntermediateDir() + "\\" + mapReduceConfig.getMapTempOutputFolder();
		std::string outputReduceDirectory = mapReduceConfig.getIntermediateDir() + "\\" + mapReduceConfig.getReduceTempOutputFolder();
		std::string mapDLLLocation = mapReduceConfig.getMapDllLocation();
		std::string reduceDLLLocation = mapReduceConfig.getReduceDllLocation();
		if (!MapStepDLL(mapDLLLocation, inputMapDirectory, outputMapDirectory)) return false;

	}
	else
	{
		return false;
	}
	*/

	// SORA UNCOMMENT OUTif (!mapSorter.sortMappedFiles(outputMapDirectory, outputReduceDirectory, sortedFileName))
	// SORA UNCOMMENT OUT{
	// SORA UNCOMMENT OUT	std::cout   << __func__ <<  "ERROR: Unable to Sort Mapped Files Output" << std::endl;
	// SORA UNCOMMENT OUT	return false;
	// SORA UNCOMMENT OUT}

	// SORA UNCOMMENT OUT if (!ReduceStepDLL(reduceDLLLocation, outputReduceDirectory, sortedFileName, outputFileName))
	// {
	//	std::cout   << __func__ <<  "ERROR: Unable to Reduce Mapped Files Output" << std::endl;
	//	return false;
	// }

	// ABI TODO: Call Finialize Class/Method

	std::string temp, temp2, temp3, temp4;
	ReduceStepDLL("", "", "", temp4);
	return true;
}

bool MapReducer::MapStepDLL(std::string& dllLocation, const std::string& inputMapDirectory, const std::string& outputMapDirectory)
{
	std::vector<std::vector<std::string>> fileListVector;
	std::vector<std::string> CompletefileList;
	std::string mapDLLLocation = dllLocation;
	uint32_t bufferSize = 3000;
	std::mutex mtx; //common mutex used to lock writes to file
	static std::condition_variable cond;
	//static std::queue<int> q;

	bool results = true;
		
	// For the input Directory read the list of files 
	fileManager.getListOfTextFiles(mapReduceConfig.getInputDir(), CompletefileList);
	uint32_t totalMapThreads = mapReduceConfig.getNumberOfMapThreads();
	uint32_t totalReduceThreads = mapReduceConfig.getNumberOfReduceThreads();
		
	// setup fileListVector
	for (size_t count = 0; count < totalMapThreads; count++)
	{
		std::vector<std::string> emptyPlaceHolderVector;
		fileListVector.push_back(emptyPlaceHolderVector);
	}

	// Split up Total File list based on the number of Map Threads
	uint32_t currentMapThread = 0;
	for (std::string file : CompletefileList)
	{
		fileListVector.at(currentMapThread).push_back(file);
		currentMapThread++;
		if (currentMapThread % totalMapThreads == 0)
		{
			currentMapThread = 0;
		}
	}

	// Launch Map Threads
	for (uint32_t Mthreads = 0; Mthreads < totalMapThreads; Mthreads++)
	{
		std::cout   << __func__ <<  "INFO: Launching Map Thread #" << Mthreads << std::endl;
		std::string threadID = "m" + std::to_string(Mthreads);
		std::vector<std::string> fileList = fileListVector.at(Mthreads);

		//void MapThreadFunction(std::string dllLocation, std::string inputDirectory, std::string outputMapDirectory, std::vector<std::string>&fileList, uint32_t bufferSize, std::string threadname, );

		std::thread mapThread(MapThreadFunction, mapDLLLocation, inputMapDirectory, outputMapDirectory, fileList, bufferSize, threadID, totalReduceThreads, ref(mtx), ref(cond));
		mapThreadList.push_back(std::move(mapThread));
	}

	// CONDITONAL VARIABLE LOGIC
	// ESA TODO: setup condition_Variable to handle async writes to MapOutput




	// JOIN THREADS LOGIC
	for (uint32_t Mthreads = 0; Mthreads < totalMapThreads; Mthreads++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		mapThreadList[Mthreads].join();
	}

	// Verify all Map Files were created

	return results;

}

/*exportSuccess is a method within Reduce CLASS which executes when the entire process has completed
it will write an output file named success.txt with the tempVector in it
it will use IO management class to push the file to the output directory*/
void MapReducer::exportSuccess()
{
	//write success and export it to an output file
	//after the entire input vector has been reduced and outputed.

	std::string fileName = "success.txt";
	std::vector<std::string> tempVector;
	tempVector.push_back("");
	fileManager.writeVectorToFile(mapReduceConfig.getOutputDir(), fileName, tempVector);
	return;
}


bool MapReducer::ReduceStepDLL(const std::string& dllLocaiton, const std::string& outputMapDirectory, const std::string& outputReduceDirectory, std::string& outputFileName)
{
	// CONSTANTS FOR TESTING JUST A PIECE
	std::string outputMapDirectory2 = "..\\WorkingDir_Default\\MapOutput";
	std::string outputReduceDir = "..\\WorkingDir_Default\\SorterOutput";
	std::string reduceDLLLocation = ".\\..\\ReduceDLL\\ReduceLib\\x64\\Debug\\ReduceLib.dll";
	uint32_t numberofReduceThreads = 2;


	bool result = true;
	std::vector<std::vector<std::string>> fileListVector;

	// Get File List for Reducer Threads
	for (uint32_t Rthreads = 0; Rthreads < numberofReduceThreads; Rthreads++)
	{
		std::vector<std::string> fileList;
		std::string startingSubString = "r" + std::to_string(Rthreads) + "_";
		fileManager.getListOfTextFilesBasedOnStart(outputMapDirectory2, startingSubString, fileList);
		fileListVector.push_back(fileList);
	}

	// Launch Reducer Threads
	for (uint32_t Rthreads = 0; Rthreads < numberofReduceThreads; Rthreads++)
	{
		std::cout   << __func__ <<  "INFO: Launching Reducer Thread #" << Rthreads << std::endl;
		std::string threadID = "r" + std::to_string(Rthreads);
		std::vector<std::string> fileList = fileListVector.at(Rthreads);
		std::thread reduceThread(&ReduceThreadFunction, reduceDLLLocation, outputReduceDir, fileListVector.at(Rthreads), threadID, outputMapDirectory2);

		reduceThreadList.push_back(std::move(reduceThread));
	}


	std::this_thread::sleep_for(std::chrono::milliseconds(15000));

	for (uint32_t Rthreads = 0; Rthreads < numberofReduceThreads; Rthreads++)
	{
		std::cout   << __func__ <<  "INFO: Attempting to Join Reducer Thread " << Rthreads << std::endl;

		if (reduceThreadList.at(Rthreads).joinable() == true)
		{
			reduceThreadList.at(Rthreads).join();
		}
	}

	return result;
}


void MapThreadFunction(std::string dllLocation, std::string inputDirectory, std::string outputMapDirectory, std::vector<std::string> fileList, uint32_t bufferSize, std::string threadname, uint32_t totalReduceThreads, std::mutex &mtx, std::condition_variable& cond)
{
	HINSTANCE hdllMap = NULL;
	pvFunctv CreateMap;
	MapInterface* mapIF = NULL;
	FileIOManagement fileManager;
	//static std::condition_variable cond;
	//static std::queue<int> q;


	hdllMap = LoadLibraryA(dllLocation.c_str());
	if (hdllMap != NULL)
	{
		CreateMap = (pvFunctv)(GetProcAddress(hdllMap, "CreateMapClassInstance"));
		if (CreateMap == nullptr)
		{
			std::cout   << __func__ <<  "Error: Did not load CreateMapClassInstance correctly." << std::endl;
		}
		else
		{
			mapIF = (static_cast<MapInterface*> (CreateMap()));	// get pointer to object
			if (mapIF == nullptr)
			{
				std::cout   << __func__ <<  "Error: Map Interface Not implemented correctly " << std::endl;
			}
			else
			{
				mapIF->setParameters(outputMapDirectory, bufferSize, totalReduceThreads);

				// Input Processing and Map Call for Each file 
				for (size_t fileCount = 0; fileCount < fileList.size(); fileCount++)
				{
					std::vector<std::string> lines;
					// Read Each File Into a Vector of Strings 
					if (fileManager.readFileIntoVector(inputDirectory, fileList.at(fileCount), lines))
					{
						// Call the Map Function for each Line
						for (size_t fileLine = 0; fileLine < lines.size(); fileLine++)
						{
							uint32_t count = 0;
							//Map Function --> Map
							//ESA Note: Conditional_variable May not be necessary for Map since writes can process in any order once lock is avail
							mapIF->createMap(fileList.at(fileCount), lines.at(fileLine), ref(mtx));
						}

						//Map Function --> Export
						mapIF->flushMap(fileList.at(fileCount), ref(mtx));
						lines.resize(0);
					}
				}
			}
		}
	}
	else
	{
		std::cout   << __func__ <<  "Error: Map Library load failed!" << std::endl;
	}
}


void ReduceThreadFunction(std::string ReduceDllLocation, std::string outputReduceDirectory, std::vector<std::string> fileList, std::string threadID, std::string MapFilesDirectory) 
{

	HINSTANCE hdllReduce = NULL;
	pvFunctv CreateReduce;
	hdllReduce = LoadLibraryA(ReduceDllLocation.c_str());
	if (hdllReduce != NULL)
	{
		CreateReduce = (pvFunctv)(GetProcAddress(hdllReduce, "CreateReduceClassInstance"));
		if (CreateReduce != nullptr)
		{
			ReduceInterface* piReduce = NULL;
			piReduce = static_cast<ReduceInterface*> (CreateReduce());	// get pointer to object
			if (piReduce != NULL)
			{

				piReduce->setParameters(outputReduceDirectory, threadID);
				for (auto file : fileList)
				{
					piReduce->reduceFile(MapFilesDirectory, file);
				}
				piReduce->exportResults();
			}
			else
			{
				std::cout   << __func__ <<  "Error: Could not create ReduceInterface Class." << std::endl;
			}
		}
		else
		{
			std::cout   << __func__ <<  "Error: Could not create ReduceInterface Class." << std::endl;
		}

	}
	else
	{
		std::cout   << __func__ <<  "Error: Reduce Library load failed!" << std::endl;
	}
}
