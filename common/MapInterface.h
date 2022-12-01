#pragma once

#include <string>
#include <mutex>

using std::string;

class MapInterface
{
public:

	//tokenizes words, accepts a key(filename) and value(single line) from fileIO
	virtual bool createMap(string filename, string inputLine) = 0;

	//clears Maps contents, prepares to read in new file
	virtual bool flushMap(const string filename) = 0;

	// converts a string into lowercase
	virtual string lowerCaseMap(const string&) = 0;

	virtual void setParameters(const string intermediate, size_t sizeOfBuffer, size_t R_threads) = 0;

	virtual string printParameters(const string&) = 0;

	virtual void ProofDLLWorks() =0;
};