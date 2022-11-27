//ReduceLib.h - contains declarations of reduce class
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "FileIO.h"
#include "framework.h"
#include "ReduceInterface.h"

using std::vector;
using std::string;

typedef std::pair<string, int> vec;

class Reduce : public ReduceInterface 
{
public:

    //variables, method titles, constructors and destructor
    Reduce();

    Reduce(std::string OutputDirectory);
    //constructor

    Reduce(const string tMemory, size_t bufferSize);
    //with buffer memory

    ~Reduce();
    //destructor

    bool reduceFile(const std::string& folderPath, const std::string& fileName);
    //import data from sorter class
    //pass 

    void setParameters(std::string OutputDirectory, std::string ThreadName);

protected:

    bool reduce(const std::string inputString);

    bool parseLine(const std::string line, std::pair<std::string, uint32_t>& outputPair);

    bool getNumberOfInstances(std::string format, std::string delim, uint32_t& instance);

    bool Export(const std::string& fileName, std::pair<std::string, uint32_t>& outputPair);

    void exportSuccess();

    bool AddFileContentsToSorter(const std::string& folderPath, const std::string& fileName);

    bool AddPhraseToMap(const std::string& formattedWord, const std::string& startString, const std::string& endString);

    bool IsolateWord(const std::string& formattedWord, const std::string& startString, const std::string& endString, std::string& isloatedWord);

private:

    std::string outputDirectory;
    size_t bufferLimit; //memory limits
    string intermediateDirectory; //saved to buffer
    string threadName;
    FileIOManagement IO_management; //using fileio class
    const std::string outputFileName = "ReducerOutput.txt";;

    /// <summary>
    /// Collection of Data for Usage. The Key is a String while the Value is the usage count 
    /// </summary>
    std::map<std::string, uint32_t> sortedMap;


};

