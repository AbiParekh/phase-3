#pragma once
//header file for FINAL CLASS

#ifndef FINAL_H
#define FINAL_H

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
#include <iostream>
#include <ostream>
#include "FileIO.h"

typedef std::pair<string, int> finVec;
//typedef std::vector<string> listOfFiles;

class Final {
public:

    //constructor
    Final();

    //final directory will take in type string
    Final(std::string FinalDir);

    //intermediate memory 
    Final(const string tMemory, size_t size_of_buffer);

    //destructor
    ~Final();

    //setting up final directory
    void setUpFinalDirectory(std::string FinalDirIn);

    bool mergeFromReduce(const std::string& folderPath, const std::string& fileName, std::string& finalVec);

    //function that attaches the merge and export
    //void finalFile(const string& folderpath, const string& filename, std::string& finalOutputFile);

    //export final output file
    //void exportFinalResults();

protected:

    //bring in data from reduced files
    //bool mergeFromReduce(const std::string& folderPath, const std::string& fileName, std::string& finalVec);

    bool getFileList(const std::string& inputFolder, std::vector<std::string>& fileList);

    //join all data items into one vector, adding lines/phrase to map
    //void mergeContent(const string& folderpath, const string& filename);

    bool exportFinal(const std::string& fileName, std::vector<std::string> finVec);

    bool finalFile(const std::string line);

    //export a file with success written in it.
    void exSuccess();

private:
    std::string finalOutputDirectory;
    size_t bufferLimit;
    string intermediateDirectory;
    FileIOManagement IO_MAN; //using fileIO class
    const std::string finalOutputFileName = "FinalOutput.txt";
    //std::map<std::string, uint32_t> reducedItems;
};

#endif //FINAL_H
