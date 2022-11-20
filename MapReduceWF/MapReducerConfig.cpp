#include "MapReducerConfig.h"
#include <iostream>
#include <string>

MapReducerConfig::MapReducerConfig() :
	inputDirectory_(""),
	intermediateDirectory_(""),
	outputDirectory_(""),
	mapDllLocation_(""),
	reduceDllLocation_(""),
	numberOfMapThreads_(0),
	numberOfReduceThreads_(0) {}

bool MapReducerConfig::parseConfigurationFile(std::string locationOfConfigurationFile)
{

}

bool MapReducerConfig::validateDirectories()
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

bool MapReducerConfig::setDefaultDirectory(std::string defaultDir)
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
}


/// GET AND SETTTERS BELOW
std::string MapReducerConfig::getInputDir()
{
	return inputDirectory_;
}

std::string MapReducerConfig::getOutputDir()
{
	return outputDirectory_;
}

std::string MapReducerConfig::getIntermediateDir()
{
	return intermediateDirectory_;
}

std::string MapReducerConfig::getMapDllLocation()
{
	return mapDllLocation_;
}

std::string MapReducerConfig::getReduceDllLocation()
{
	return reduceDllLocation_;
}

std::string MapReducerConfig::getMapTempOutputFolder()
{
	return folderNameForMapOutput;
}

std::string MapReducerConfig::getReduceTempOutputFolder()
{
	return folderNameForSorterOutput;
}

uint32_t MapReducerConfig::getNumberOfMapThreads()
{
	return numberOfMapThreads_;
}

uint32_t MapReducerConfig::getNumberOfReduceThreads()
{
	return numberOfReduceThreads_;
}

void MapReducerConfig::setInputDir(std::string in)
{
	inputDirectory_ = in;
}

void MapReducerConfig::setOutputDir(std::string out)
{
	outputDirectory_ = out;
}

void MapReducerConfig::setIntermediateDir(std::string middle)
{
	intermediateDirectory_ = middle;
}

void MapReducerConfig::setMapDllLocation(std::string out)
{
	mapDllLocation_ = out;
}

void MapReducerConfig::setReduceDllLocation(std::string middle)
{
	reduceDllLocation_ = middle;
}

void MapReducerConfig::setNumberOfMapThreads(uint32_t threadNumber)
{
	numberOfMapThreads_ = threadNumber;
}

void MapReducerConfig::setNumberOfReduceThreads(uint32_t threadNumber)
{
	numberOfReduceThreads_ = threadNumber;
}