// mapReduceCMake.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <vector>
#include <string>
#include "MapReduceSorter.h"
#include "FileIO.h"
#include "MapReducerConfig.h"
#include "ReduceInterface.h"
#include "MapInterface.h"

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

	// Variables with Map Reducer

	const std::size_t bufferSize{ 3000 };

	std::string configurationFileLocation_;

	MapReducerConfig mapReduceConfig;
	
	// Map Object 
	FileIOManagement fileManager;

	// 
	MapSorter mapSorter;

};