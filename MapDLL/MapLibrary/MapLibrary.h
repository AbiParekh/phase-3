#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "FileIO.h"
#include "framework.h"
#include "MapInterface.h"
#include <variant>
#include <mutex>
#include <queue>

using std::string;
using std::vector;
using std::ostream;

//tokenPair Type used to create a key,value pair of words, count
typedef std::pair<string, int> tokenPair;


//class MAPLIBRARY_API Map
class Map : public MapInterface
{
public:

	//default constructor if no Directories nor buffer set
	Map();

	//default constructor if no buffer size set
	Map(const string intermediateDir);

	//constructor with buffersize set
	Map(const string intermediateDir, size_t sizeOfBuffer);

	//Destructor
	~Map();

	//Copy
	Map(const Map& t);

	//tokenizes words, accepts a key(filename) and value(single line) from fileIO
	 bool createMap(string filename, string inputLine);

	//clears Maps contents, prepares to read in new file
	bool flushMap(const string filename);

	// converts a string into lowercase
	string lowerCaseMap(const string&);

	void setParameters(const string intermediate, size_t sizeOfBuffer, size_t R_threads);

	string printParameters(const string&);
	
	void ProofDLLWorks();

protected: // PRIVATE MEMBER FUNCTIONS 

	//empties buffer
	bool emptyCache();

	//accepts key(filename) and value(vector of tokenized strings) sends to output when buffer is full
	bool exportMap(string filename, string token);

	//checks each character to remove punctuation from word, validates apostrophe as char
	bool removePunctuation(const string str, const int tokenStart, const int tokenEnd);

	// adds suffix to end of intermediate file
	string addFileSuffix(const string filename, int index);

private: // PRIVATE DATA MEMBERS 


	//number of Reducer threads to run
	size_t R_threads;

	//size of buffer, exports to tempDirectory if full
	size_t maxBufferSize{};

	//formatted as string of tokens ("token1",1),("token2",2),... 

	vector<vector<string>> exportBuffers;
	vector<int> exportThread;
	
	//private data member tokenPair ("",int)

	vector<tokenPair> tokenWords; //takes filename as string and tokenpairs
	vector<vector<tokenPair>>R_tokenWords{ R_threads };
	//FilePaths passed as args from main
	string tempDirectory;

	//handles FileIO implementation
	FileIOManagement mapFileManager;

	//Counter to index large files broken into smaller tempFiles
	int fileIndex{ 0 };


};
