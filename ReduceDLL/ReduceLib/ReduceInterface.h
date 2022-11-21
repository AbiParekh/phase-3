#pragma once

#include <string>

using std::string;

class ReduceInterface
{
public:

	virtual bool reduceFile(const std::string& folderPath, const std::string& fileName, std::string& outFile) = 0;

	virtual void ProofDLLWorks() = 0;

	virtual void setParameters(std::string OutputDirectory) = 0;

};