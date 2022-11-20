// mapReduceCMake.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <vector>
#include <string>
#include "MapReduceSorter.h"
#include "FileIO.h"
#include "ReduceInterface.h"
#include "MapInterface.h"

typedef void* (*pvFunctv)();

class MapReducer
{

public:

	// Constructor with inputs
	MapReducer(std::string mapDll, std::string reduceDll, std::string inputDir, std::string outputDir, std::string middleDir);

	bool reduce(std::string& outputFileName);

	std::string getInputDir();

	std::string getOutputDir();

	std::string getIntermediateDir();

	void setInputDir(std::string);

	void setOutputDir(std::string);

	void setIntermediateDir(std::string);

	bool setDefaultDirectory(std::string);

private:

	/// <summary>
	/// Validate the Directories required for Reduction 
	/// </summary>
	/// S-OConnor TODO Fill Out Comment
	/// <returns></returns>
	bool validateDirectories();

	bool doReduce(std::string& outputFileName);

	bool MapStepDLL(std::string& dllLocaiton, const std::string& outputMapDirectory);

	bool ReduceStepDLL(const std::string& dllLocaiton, const std::string& outputSortDirectory, const std::string&, std::string&);

	// Variables with Map Reducer
	const std::string folderNameForMapOutput = "MapOutput";

	const std::string folderNameForSorterOutput = "SorterOutput";

	const std::size_t bufferSize{ 3000 };

	// Input Argument provide at time of construction with the location of the input files
	std::string inputDirectory_;

	// Input Argument provide at time of construction with the location of an existing temporary directory for storing the final output
	std::string outputDirectory_;

	// Input Argument provide at time of construction with the location of an existing temporary directory for storing working files
	std::string intermediateDirectory_;

	std::string mapDllLocation_;

	std::string reduceDllLocation_;
	// Map Object 
	FileIOManagement fileManager;

	// 
	MapSorter mapSorter;

};