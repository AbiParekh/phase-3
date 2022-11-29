// MathLibrary.cpp : Defines the exported functions for the DLL.


#include "MapLibrary.h"


Map::Map()
	//:maxBufferSize{ 1024 } // defaults sizeOfBuffer to 1k tokenPairs
{};

Map::~Map()
{
	//To do: deallocate memory in vectors, tokenPairs
};

Map::Map(const string intermediate)
	: tempDirectory{ intermediate }, maxBufferSize{ 10 }
{};

Map::Map(const string intermediate, size_t sizeOfBuffer)
	: tempDirectory{ intermediate }, maxBufferSize{ sizeOfBuffer }
{};

Map::Map(const Map& t)
{
	maxBufferSize = t.maxBufferSize;
	exportBuffers = t.exportBuffers;
	tokenWords = t.tokenWords;
	tempDirectory = t.tempDirectory;
	mapFileManager = t.mapFileManager;
	fileIndex = t.fileIndex;

}

void Map::ProofDLLWorks()
{
	std::cout << "WELCOME TO THE THUNDERDOME" << std::endl;
	std::cout << "Your A WIZARD HARRY" << std::endl;
}

void Map::setParameters(const string intermediateIn, size_t sizeOfBufferIn, size_t threads)
{
	tempDirectory = intermediateIn;
	maxBufferSize = sizeOfBufferIn;
	R_threads = threads;
}


string Map::printParameters(const string& input)
{

	string test = input;
	size_t testI = R_threads;
	test = "R_threads = " + std::to_string(R_threads);
	return test;
}

bool Map::removePunctuation(const string str, const int tokenStart, const int tokenEnd)
{
	//not alphanumeric and not apostrophe, must be punctuation
	if (!iswalnum(str[tokenEnd]) && !(str[tokenEnd] == '\''))
	{
		return true;
	}

	else if (tokenEnd == str.length())
	{
		return true;
	}

	//determine if apostrophe should be char or punct
	else if (str[tokenEnd] == '\'')
	{
		//apostrophe is first char in word, treat as punctuation
		if (tokenStart == tokenEnd)
		{
			return true;
		}

		//apostrophe is last char in word, check if ok to keep
		if (!iswalnum(str[tokenEnd + 1]))
		{
			//apostrophe ok since contracts word, i.e class' or o' clock
			if (str[tokenEnd - 1] == 's' || str[tokenEnd - 1] == 'o')
			{
				return false;
			}
			//apostrophe must be a punctuation
			else
			{
				return true;
			}
		}
	}
	return false;
}

string Map::addFileSuffix(const string filename, int index)
{
	string tempFile = filename;
	size_t lastdot = filename.find_last_of("."); //finds extension if any in file
	if (lastdot == std::string::npos) //filename has no extensions
	{
		return filename + '_' + std::to_string(index);
	}
	else //need to append index before extension from file
	{
		return filename.substr(0, lastdot) + '_' + std::to_string(index) + filename.substr(lastdot);
	}
}

string Map::lowerCaseMap(const string& input)
{
	string output;
	for (char c : input)
	{
		output += tolower(c);
	}
	return output;
}

bool Map::createMap(const string filename, const string strCAPS)
{
	bool isExported{ false };
	string parsedWord, str{ lowerCaseMap(strCAPS) };

	for (int tokenStart = 0, tokenEnd = 0; tokenEnd <= str.length(); tokenEnd++) //iterate through each char, check if end of work
	{
		//((!iswalnum(str[tokenEnd]) && !(str[tokenEnd] == '\'')) || tokenEnd == str.length()) //checks if char is not alphanumeric (iswalnum) or apostrophe
		if (removePunctuation(str, tokenStart, tokenEnd))
		{
			if (tokenStart != tokenEnd) //not first char in word
			{
				parsedWord = str.substr(tokenStart, tokenEnd - tokenStart);
				isExported = exportMap(filename, parsedWord);
				tokenStart = tokenEnd + 1;	// moves word start to next char
			}
			else if (tokenStart == tokenEnd) //first char in word is a punct
			{
				tokenStart = tokenEnd + 1; // moves word start to next char
			}
		}
	}
	return isExported;
};

bool Map::flushMap(const string fileName)
{
	bool isFlushed = exportMap(fileName, "");
	std::cout << "Mapped " << fileIndex << " Partition(s) of " << fileName << " to tempDirectory: " << this->tempDirectory << std::endl;
	fileIndex = 0;
	return isFlushed; //nothing to flush
}

bool Map::emptyCache()
{
	bool isEmptied{ false };

	if (tokenWords.size() > 0)
	{
		exportBuffers.resize(0);
		size_t index = 0; 

		for (int i = 0; i < R_threads; i++)
		{
			exportBuffers.push_back({});
		}

		for (int i = 0, R_Num; i < tokenWords.size(); i++)
		{
			R_Num = static_cast<int> (tokenWords[i].first[0]) % R_threads; //adds R_thread process to vector
			exportBuffers[R_Num].push_back("(\"" + tokenWords[i].first + "\"," + std::to_string(tokenWords[i].second) + ")");
		}

		tokenWords.erase(tokenWords.begin(), tokenWords.begin() + tokenWords.size());
		isEmptied = true;
	}
	return isEmptied; //False if no cache emptied
}

bool Map::exportMap(const string filename, string token)
{
	bool isExported{ false };

	//does not execute on flushMap
	if (token != "") tokenWords.push_back(std::make_pair(token, 1)); 

	// Buffer reached or flush called dump to FileIO
	if (tokenWords.size() == maxBufferSize || token == "") 
	{
		bool isExported = emptyCache();
		vector<string> tempFiles;

		//Prevents duplicate write to file if current buffer was already exported
		if (isExported) {
			string tempFile = addFileSuffix(filename, fileIndex);
			fileIndex++;

			//writes contents of buffer to file in temp directory
			for (int R = 0; R < R_threads; R++)
			{
				tempFiles.push_back("r" + std::to_string(R) + "_" + filename);
				mapFileManager.writeVectorToFile(this->tempDirectory, tempFiles[R], exportBuffers[R], true);
			}
		}
	}
	return isExported; //False if nothing exported
}

