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
	numberOfReduceThreads_(0),
	mapBufferSize_(0){}

bool MapReducerConfig::parseConfigurationFile(std::string locationOfConfigurationFile)
{		
	bool results = true;
	std::vector<std::string> items;

	if (!fileManager.readFileIntoVector(locationOfConfigurationFile, items))
	{
		results = false;
	}
	else
	{
		for (std::string item : items)
		{
			if (!parseConfigurationLine(item))
			{
				std::cout   << __func__ <<  "ERROR: Unable to parse Configuration Item: " << item << std::endl;
			}
		}
		if (!requiredConfigurationItemsPresent())
		{
			std::cout   << __func__ <<  "ERROR: Configuration Files is Missing Parameters" << std::endl;
			results = false;
		}
		else if (!validateDirectories())
		{
			std::cout   << __func__ <<  "ERROR: Configuration Settings were not valid" << std::endl;
			results = false;
		}
	}
	return results;
}

bool MapReducerConfig::validateDirectories()
{
	//Check Input Directory
	bool results = fileManager.validDirectory(inputDirectory_);
	// SROA: Determine if Directory Existing if it does and its not valid
	if (results == false)
	{
		std::cout   << __func__ <<  "ERROR: You must specify a valid input Directory" << std::endl;
		return results;
	}

	results = results && fileManager.validDirectory(outputDirectory_);
	if (results == false)
	{
		std::cout   << __func__ <<  "ERROR: You must specify a valid Output Directory" << std::endl;
		return results;
	}

	//Check Intermediate Directory
	results = results && fileManager.validDirectory(intermediateDirectory_);
	if (results == false)
	{
		std::cout   << __func__ <<  "ERROR: You must specify a valid Intermediate Directory" << std::endl;
		return results;
	}


	// Create Working Directories within the Intermediate Directory
	if (results == true)
	{
		std::string mapDirName = intermediateDirectory_ + "\\" + folderNameForMapOutput;
		if (fileManager.validDirectory(mapDirName)== true)
		{
			std::vector<std::string> fileList;
			fileManager.getListOfTextFiles(mapDirName, fileList);
			for (size_t count = 0; count < fileList.size(); count++)
			{
				// Stuff exist in output directory...... kill it
				std::string totalFilePath = mapDirName + "\\" + fileList.at(count);
				if (!fileManager.deleteFile(totalFilePath))
				{
					std::cout << __func__ << "ERROR: Unable to Delete (" << totalFilePath << ") in the Output Directory" << std::endl;
					results = false;
				}
			}
		}
		else
		{
			results = results && fileManager.createDirectory(intermediateDirectory_, folderNameForMapOutput);
		}

		std::string reduceDirName = intermediateDirectory_ + "\\" + folderNameForReducerOutput;

		if (fileManager.validDirectory(reduceDirName) == true)
		{
			std::vector<std::string> fileList;
			fileManager.getListOfTextFiles(reduceDirName, fileList);
			for (size_t count = 0; count < fileList.size(); count++)
			{
				// Stuff exist in output directory...... kill it
				std::string totalFilePath = reduceDirName + "\\" + fileList.at(count);
				if (!fileManager.deleteFile(totalFilePath))
				{
					std::cout << __func__ << "ERROR: Unable to Delete (" << totalFilePath << ") in the Output Directory" << std::endl;
					results = false;
				}
			}
		}
		else
		{
			results = results && fileManager.createDirectory(intermediateDirectory_, folderNameForReducerOutput);
		}

		if (results == false)
		{
			std::cout   << __func__ <<  "ERROR: Couldn't create MapOutput or ReducerOutput, Terminating" << std::endl;
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
				std::cout   << __func__ <<  "ERROR: Unable to Delete (" << totalFilePath << ") in the Output Directory" << std::endl;
				results = false;
			}
		}
	}
	return results;
}

bool MapReducerConfig::setDefaultDirectory(const std::string defaultDir, std::string& finalDirectoryName)
{
	finalDirectoryName = "..\\" + defaultDir + "Dir_Default";
	bool results = true;
	results = fileManager.validDirectory(finalDirectoryName);

	// check if a default directory exists
	if (results == false)
	{
		results = fileManager.createDirectory("..\\", defaultDir + "Dir_Default"); //temp directory in working folder
		if (results == false)
		{
			std::cout << __func__ << "Terminating: Failed to create default " + defaultDir + " directory" << std::endl;
			return results;
		}
		std::cout << __func__ << "INFO: New defaultDir directory ..\\" + defaultDir + "Dir_Default created successfully!" << std::endl;
	}
		return results;
}

bool MapReducerConfig::parseConfigurationLine(std::string line)
{
	std::string configurationParameter;
	std::string configurationValue;
	
	// Break Configuration File into Parameter and Value
	std::size_t pos = line.find(" ");
	if(pos == std::string::npos)
	{
		std::cout   << __func__ <<  "ERROR: \"" << line << "\" Not Formatted Correctly (Single Word)"  << std::endl;
		return false;
	}
	configurationParameter = line.substr(0, pos);
	configurationValue = line.substr(pos+1);
#ifdef false
	std::cout   << __func__ <<  "configurationParameter: '" << configurationParameter << "' configurationValue: '" << configurationValue << "'" << std::endl;
#endif
	if(configurationValue.find(" ") != std::string::npos)
	{	
		std::cout   << __func__ <<  "ERROR: \"" << line << "\" Not Formatted Correctly (More then 2 Words in a Single Line)" << std::endl;
		return false;
	}

	if (configurationParameter.compare("Input_Directory") == 0)
	{
		inputDirectory_ = configurationValue;
	}
	else if(configurationParameter.compare("Map_DLL_Location")			== 0) mapDllLocation_			= configurationValue;
	else if(configurationParameter.compare("Reduce_DLL_Location")		== 0) reduceDllLocation_		= configurationValue;
	else if(configurationParameter.compare("Output_Directory")			== 0) outputDirectory_			= configurationValue;
	else if(configurationParameter.compare("Temp_Directory")			== 0) intermediateDirectory_	= configurationValue;
	else if(configurationParameter.compare("Number_Of_Map_Threads")		== 0) numberOfMapThreads_		= std::stoul(configurationValue, nullptr, 0);
	else if(configurationParameter.compare("Number_Of_Reduce_Threads")	== 0) numberOfReduceThreads_	= std::stoul(configurationValue, nullptr, 0);
	else if (configurationParameter.compare("Map_Buffer_Size")			== 0) mapBufferSize_			= std::stoul(configurationValue, nullptr, 0);
	else
	{
		std::cout   << __func__ <<  "ERROR: Unknown Configuration Parameter (" << configurationParameter << ") with the value (" << configurationValue << ")" << std::endl;
	}
	return true;
}

bool MapReducerConfig::requiredConfigurationItemsPresent()
{
	bool results = true;
	if (inputDirectory_.compare("") ==0)
	{
		std::cout   << __func__ <<  " ERROR: Input Directory not identified in Configuration File" << std::endl;
		results = false;
	}

	if (mapDllLocation_.compare("") == 0)
	{
		std::cout   << __func__ <<  " ERROR: Map DLL Location not identified in Configuration File" << std::endl;
		results = false;
	}

	if (reduceDllLocation_.compare("") == 0)
	{
		std::cout   << __func__ <<  " ERROR: Reduce DLL Location not identified in Configuration File" << std::endl;
		results = false;
	}

	if (outputDirectory_.compare("") == 0) 
	{  // If User did not provided output Dir, then it should be created by default as it is an optional parameter
//		std::cout   << __func__ <<  "INFO: Optional Parameter Output Directory Not Set" << std::endl;
		if (setDefaultDirectory("Output", outputDirectory_) == false)
		{
			std::cout   << __func__ <<  " ERROR: Unable to est Output Directory. " << std::endl;
			results =  false;
		};
	}
	else
	{
		std::vector<std::string> fileList;
		fileManager.getListOfTextFiles(outputDirectory_, fileList);
		for (size_t count = 0; count < fileList.size(); count++)
		{
			// Stuff exist in output directory...... kill it
			std::string totalFilePath = outputDirectory_ + "\\" + fileList.at(count);
			if (!fileManager.deleteFile(totalFilePath))
			{
				std::cout << __func__ << " ERROR: Unable to Delete (" << totalFilePath << ") in the Output Directory" << std::endl;
				results = false;
			}
		}
	}
	
 
	if (intermediateDirectory_.compare("") == 0)  
	{   // If User did not provided IntermediateDirectory, then it should be created by default as it is an optional parameter
//		std::cout   << __func__ <<  "INFO: Optional Parameter Intermediate Directory Not Set" << std::endl;
		if (setDefaultDirectory("Working", intermediateDirectory_) == false)
		{
			std::cout   << __func__ <<  " ERROR: Unable to est intermediatw Directory. " << std::endl;
			results = false;
		};

	}

	if (numberOfMapThreads_ == 0)
	{
		std::cout   << __func__ <<  " ERROR: Map Threads not identified in Configuration File" << std::endl;
		results = false;
	}
	if (numberOfReduceThreads_ == 0)
	{
		std::cout   << __func__ <<  " ERROR: Number of Reduce Threads not identified in Configuration File" << std::endl;
		results = false;
	}
	if (mapBufferSize_ == 0)
	{
		std::cout << __func__ << " ERROR: Buffer Size missing from Configuration File. Try: Map_Buffer_Size 3000" << std::endl;
		results = false;
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
	return folderNameForReducerOutput;
}

uint32_t MapReducerConfig::getNumberOfMapThreads()
{
	return numberOfMapThreads_;
}

uint32_t MapReducerConfig::getNumberOfReduceThreads()
{
	return numberOfReduceThreads_;
}

uint32_t MapReducerConfig::getMapBufferSize()
{
	return mapBufferSize_;
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

void MapReducerConfig::setMapBufferSize(uint32_t bufferSize)
{
	mapBufferSize_ = bufferSize;
}