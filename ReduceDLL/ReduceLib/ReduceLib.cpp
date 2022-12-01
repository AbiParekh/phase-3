#include "ReduceLib.h"

/*Takes an input with a key and its respective values ("for, [1,1,1,]")
    Sums up all values for each key, and returns a vector with all reduced values
    export function writes out to a file with the reduced values
    output a file written with success at the end.*/

Reduce::Reduce()
    : bufferLimit(2048) {}

Reduce::Reduce(std::string OutputDir)
    : bufferLimit(2048),
    outputDirectory(OutputDir)
{}


Reduce::Reduce(const string tMemory, size_t bufferSize)
    : intermediateDirectory{ tMemory }, bufferLimit{ bufferSize }
{}

//Reduce::Reduce(const string key)


Reduce::~Reduce()
//destructor
{};

void Reduce::setParameters(std::string OutputDirIn, std::string ThreadName)
{
    outputDirectory = OutputDirIn;
    threadName = ThreadName;
}

void Reduce::getParameters(std::string& OutputDirectoryOut, std::string& ThreadNameOut)
{
    OutputDirectoryOut = outputDirectory;
    ThreadNameOut = threadName;
}

void Reduce::ProofDLLWorks()
{
    std::cout << "THIS IS PROOF THE REDUCE DLL IS WORKING!" << std::endl;
}


//from sorter grab the sorted data 
//then insert the data into an input vector, then send it to the reduce method
bool Reduce::reduceFile(const string& folderPath, const string& fileName)
{
    bool results = true;
    if (!AddFileContentsToSorter(folderPath, fileName))
    {
        std::cout   << __func__ << "Error: Unable to add File: " << folderPath << "\\" << fileName << "To Sorted List" << std::endl;
        results = false;
    }
    
    return results;
}

bool Reduce::AddFileContentsToSorter(const std::string& folderPath, const std::string& fileName)
{
    std::vector<std::string> lines;
    const std::string WORD_WRAP = "\"";
    bool result = true;

    if (IO_management.readFileIntoVector(folderPath, fileName, lines))
    {
        for (size_t fileLine = 0; fileLine < lines.size(); fileLine++)
        {
            AddPhraseToMap(lines.at(fileLine), WORD_WRAP, WORD_WRAP);
        }
    }
    else
    {
        result = false;
    }
    return result;
}

bool Reduce::AddPhraseToMap(const std::string& phrase, const std::string& startString, const std::string& endString)
{
    std::string isolateWord;
    if (IsolateWord(phrase, startString, endString, isolateWord))
    {
        std::map<std::string, uint32_t>::iterator mapIterator = sortedMap.find(isolateWord);
        if (mapIterator == sortedMap.end())
        {
            // The word was not found in the Map therefore add it 
            sortedMap.insert(std::pair<std::string, uint32_t>(isolateWord, 1));
        }
        else
        {
            // The Word was found in the map increment it
            mapIterator->second = mapIterator->second + 1;
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
bool Reduce::IsolateWord(const std::string& formattedWord, const std::string& startString, const std::string& endString, std::string& isloatedWord)
{
    size_t firstPosition = formattedWord.find(startString);
    size_t secondPosition = formattedWord.find(endString, firstPosition + 1);
    if ((firstPosition == std::string::npos) || (secondPosition == std::string::npos)) return false;
    isloatedWord = formattedWord.substr(firstPosition + 1, secondPosition - (firstPosition + 1));

    return true;
}



void Reduce::exportResults()
{
    std::vector<std::string> outputVector;
    for (std::map<std::string, uint32_t>::iterator mapIterator = sortedMap.begin(); mapIterator != sortedMap.end(); ++mapIterator)
    {
        std::string formattedOutput = "(\"" + mapIterator->first + "\", " + std::to_string(mapIterator->second) + ")";
        outputVector.push_back(formattedOutput);
    }
    std::string fileName = threadName + "_" + outputFileName;
    if (!IO_management.writeVectorToFile(outputDirectory, fileName, outputVector))
    {
        std::cout   << __func__ << "ERROR: Unable to Write Sorted Map File " << std::endl;
    }
}



