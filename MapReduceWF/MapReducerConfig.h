#pragma once
#include <string>
#include "FileIO.h"

class MapReducerConfig
{
public:
	MapReducerConfig();


	std::string getInputDir();

	std::string getOutputDir();

	std::string getIntermediateDir();

	std::string getMapDllLocation();

	std::string getReduceDllLocation();

	uint32_t getNumberOfMapThreads();

	uint32_t getNumberOfReduceThreads();

	std::string getMapTempOutputFolder();

	std::string getReduceTempOutputFolder();

	void setInputDir(std::string);

	void setOutputDir(std::string);

	void setIntermediateDir(std::string);

	void setMapDllLocation(std::string);

	void setReduceDllLocation(std::string);

	void setNumberOfMapThreads(uint32_t);

	void setNumberOfReduceThreads(uint32_t);

	bool parseConfigurationFile(std::string locationOfConfigurationFile);

protected:

	bool validateDirectories();

	bool setDefaultDirectory(std::string defaultDir);

	std::string inputDirectory_;
	std::string intermediateDirectory_;
	std::string outputDirectory_;
	std::string mapDllLocation_;
	std::string reduceDllLocation_;
	uint32_t numberOfMapThreads_;
	uint32_t numberOfReduceThreads_;

private:
	FileIOManagement fileManager;

	const std::string folderNameForMapOutput = "MapOutput";

	const std::string folderNameForSorterOutput = "SorterOutput";

}