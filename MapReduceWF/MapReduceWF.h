// mapReduceCMake.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <vector>
#include <string>
#include <thread>	
#include "MapReduceSorter.h"
#include "FileIO.h"
#include "MapReducerConfig.h"
#include "ReduceInterface.h"
#include "MapInterface.h"

typedef void* (*pvFunctv)();

void ReduceThreadFunction(std::string dllLocation, std::string outputReduceDirectory, std::vector<std::string> temp, std::string threadname, std::string MapFilesDirectory);
void MapThreadFunction(std::string dllLocation, std::string outputMapDirectory, std::vector<std::string>& fileList, std::string inputDirectory, uint32_t bufferSize);


class MapReducer
{

public:

	// Constructor with inputs
	MapReducer(std::string configFileLocation);

	bool reduce(std::string& outputFileName);


private:

	bool doReduce(std::string& outputFileName);

	bool MapStepDLL(std::string& dllLocaiton, const std::string& outputMapDirectory);


	bool ReduceStepDLL(const std::string& dllLocaiton, const std::string& outputSortDirectory, const std::string&, std::string&);

	bool MergeReduceThreads(const std::string& outputReduceDirectory);

	// Variables with Map Reducer

	const std::size_t bufferSize{ 3000 };

	std::vector<std::thread> mapThreadList;

	std::vector<std::thread> reduceThreadList;

	std::string configurationFileLocation_;

	MapReducerConfig mapReduceConfig;
	
	// Map Object 
	FileIOManagement fileManager;

	// 
	MapSorter mapSorter;

};