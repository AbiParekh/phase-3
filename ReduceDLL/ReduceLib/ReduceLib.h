//ReduceLib.h - contains declarations of reduce class
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../../common/FileIO.h"
#include "framework.h"
#include "../../common/ReduceInterface.h"

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

    void setParameters(std::string OutputDirectory, std::string ThreadName);

    void getParameters(std::string& OutputDirectory, std::string& ThreadName);

    void ProofDLLWorks();

    bool reduceFile(const std::string& folderPath, const std::string& fileName);

    void exportResults();


 protected:


    bool Export(const std::string& fileName, std::pair<std::string, uint32_t>& outputPair);

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

