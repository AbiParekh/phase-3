#pragma once
//header file for FINAL CLASS

#ifndef FINAL_H
#define FINAL_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include "../common/FileIO.h"

typedef std::pair<std::string, int> finVec;
//typedef std::vector<string> listOfFiles;

class Final {
public:

    //constructor
    Final();


    //destructor
    ~Final();

    //setting up final directory
    void setParameters(std::string reducerOutputDirectoryIn, std::string finalOutputDirectoryIn);

    bool mergeFromReduce(std::string& outputFileName);

protected:

    bool mapToVectorConversion(std::map<std::string, uint32_t> mapItems, std::vector<std::string>& finalOutputVector);

    bool updateMapWithLine(std::string line);
    
    bool IsolateWord(const std::string& formattedWord, const std::string& startString, const std::string& endString, std::string& isloatedWord, uint32_t& value);

private:
    std::string reducerOutputDirectory;
    std::string finalOutputDirectory;    
    FileIOManagement fileManager; //using fileIO class
    const std::string finalOutputFileName = "FinalOutput.txt";
    std::map<std::string, uint32_t> reducedItems;
};

#endif //FINAL_H
