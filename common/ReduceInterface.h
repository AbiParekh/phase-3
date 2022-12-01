#pragma once

#include <string>

class ReduceInterface
{
public:
	virtual bool reduceFile(const std::string& folderPath, const std::string& fileName) = 0;

	virtual void ProofDLLWorks() = 0;

	virtual void setParameters(std::string OutputDirectory, std::string ThreadName) = 0;

	virtual void exportResults() = 0;
};