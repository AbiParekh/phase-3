#pragma once

#include <string>

class MapInterface
{
public:

	//tokenizes words, accepts a key(filename) and value(single line) from fileIO
	virtual bool createMap(std::string filename, std::string inputLine) = 0;

	//clears Maps contents, prepares to read in new file
	virtual bool flushMap(const std::string filename) = 0;

	// converts a string into lowercase
	virtual std::string lowerCaseMap(const std::string&) = 0;

	virtual void setParameters(const std::string intermediate, size_t sizeOfBuffer) = 0;

	virtual std::string printParameters(const std::string&) = 0;

	virtual void ProofDLLWorks() = 0;
};