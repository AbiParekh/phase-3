//The Final class contains the main fuctions for combining the results from the Reduce class, and merging them into one output file

#include "FinalMerge.h"
#include "../common/FileIO.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <Windows.h>
//#include <filesystem>
#include <string>
#include <vector>
using std::string;
using std::vector;
using namespace std;
//namespace fs = std::filesystem;


//setting buffer
Final::Final()
{
    finalOutputDirectory = "";
    reducerOutputDirectory = "";

}

//destructor
Final::~Final()
{};


/*Sets up the FinalOutput Directory, where the a single merged output file and a success file will written to*/
void Final::setParameters(std::string reducerOutputDirectoryIn, std::string finalOutputDirectoryIn)
{
    reducerOutputDirectory = reducerOutputDirectoryIn;
    finalOutputDirectory = finalOutputDirectoryIn;
}


/*the function importFromReduce is used for merging data from multiple files into one, the data is obtained from the
Reducer Directory, and imported into the Final
*/
bool Final::mergeFromReduce(std::string& outputFileName)
{
    
    bool results = true;
    std::vector<std::string> fileList;
    fileManager.getListOfTextFiles(reducerOutputDirectory, fileList);
    for (auto file : fileList)
    {
        std::vector<std::string> stringList;
        if (fileManager.readFileIntoVector(reducerOutputDirectory, file, stringList))
        {
            for (auto line : stringList)
            {
                if (!updateMapWithLine(line))
                {
                    std::cout << "ERROR: Can't Update Final Map with Line: " << line << std::endl;
                }
            }
        }
        else
        {
            std::cout << "ERROR: Can't Read File (" << file << ") for the final Merge" << std::endl;
        }
    }
    std::vector<std::string> finalOutputVector;

    if (mapToVectorConversion(reducedItems, finalOutputVector))
    {
        outputFileName = finalOutputFileName;
        fileManager.writeVectorToFile(finalOutputDirectory, outputFileName, finalOutputVector);
    }
    
    return results;
}


bool Final::mapToVectorConversion(std::map<std::string, uint32_t> mapItems, std::vector<std::string>&  finalOutputVector)
{
    std::map<std::string, uint32_t>::iterator begIt = mapItems.begin();
    for (; begIt != mapItems.end(); ++begIt)
    {
        std::string temp;
        std::pair<std::string, uint32_t> outputPair = *begIt;
        std::string formattedOutput = "\"" + outputPair.first + "\", " + std::to_string(outputPair.second);
        finalOutputVector.push_back(formattedOutput);
    }

    return true;
}


bool Final::updateMapWithLine(std::string line)
{
    std::string isolateWord;
    const std::string WORD_WRAP = "\"";
    uint32_t value;
    if (IsolateWord(line, WORD_WRAP, WORD_WRAP, isolateWord, value))
    {
        std::map<std::string, uint32_t>::iterator mapIterator = reducedItems.find(isolateWord);
        if (mapIterator == reducedItems.end())
        {
            // The word was not found in the Map therefore add it 
            reducedItems.insert(std::pair<std::string, uint32_t>(isolateWord, value));
        }
        else
        {
            // The Word was found in the map increment it
            mapIterator->second = mapIterator->second + value;
        }
    }
    else
    {
        return false;
    }
    return true;
}


/*The IsolateWord function within the Reduce class has parameters string word, the start of the string, the end of the string, and the isolated string word
the function locates the first position of the string this assigned as the startString, and it locates the second portion of the string and it assigns this as endsString
It then formats the strings into a sub string and assigns it to the isolated string.*/
bool Final::IsolateWord(const std::string& formattedWord, const std::string& startString, const std::string& endString, std::string& isloatedWord, uint32_t& value)
{
    // Isolate Word
    size_t firstPosition = formattedWord.find(startString);
    size_t secondPosition = formattedWord.find(endString, firstPosition + 1);
    if ((firstPosition == std::string::npos) || (secondPosition == std::string::npos)) return false;
    isloatedWord = formattedWord.substr(firstPosition + 1, secondPosition - (firstPosition + 1));

    // Isolate Value
    std::string ValueString = formattedWord.substr(secondPosition);
    size_t valueFirstPosition = formattedWord.find(",");
    size_t valueSecondPosition = formattedWord.find(")", valueFirstPosition + 1);
    if ((valueFirstPosition == std::string::npos) || (valueSecondPosition == std::string::npos)) return false;
    std::string isloatedValue = formattedWord.substr(valueFirstPosition + 1, valueSecondPosition - (valueFirstPosition + 1));
    value = std::stoul(isloatedValue);
   
    return true;
}

