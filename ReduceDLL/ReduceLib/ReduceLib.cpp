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



//from sorter grab the sorted data 
//then insert the data into an input vector, then send it to the reduce method
bool Reduce::reduceFile(const string& folderPath, const string& fileName)
{
    bool results = true;
    if (!AddFileContentsToSorter(folderPath, fileName));
    {
        std::cout << "Error: Unable to add File: " << folderPath << "\\" << fileName << "To Sorted List" << std::endl;
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


/*the Export function within the Reduce class will take fileName, and vector from method reduce
it will first format the output in order to read the keys on left side and the value on the right
a vector called tempVector is initialized to hold the formatted pairs
IO management class is called to write the tempVector to the outputDirectory*/
bool Reduce::Export(const std::string& fileName, std::pair<std::string, uint32_t>& outputPair)
{
    std::string formattedOutput = "\"" + outputPair.first + "\", " + std::to_string(outputPair.second);
    std::vector<std::string> tempVector;
    tempVector.push_back(formattedOutput);
    IO_management.writeVectorToFile(outputDirectory, fileName, tempVector, true);
    return true;
}


/*the getNumberOfInstances function within Reduce class collects all the 1s for a particular keyword
and it stores the final value into the vector called instance*/
bool Reduce::getNumberOfInstances(std::string format, std::string delim, uint32_t& instance)
{
    size_t firstPosition = format.find(delim);
    if (firstPosition == std::string::npos)
    {
        return false;
    }
    size_t formatSize = format.size();
    std::string subString = format.substr(firstPosition);
    std::string::difference_type n = std::count(subString.begin(), subString.end(), '1');
    instance = static_cast<uint32_t>(n);
    return true;
}



/*exportSuccess is a method within Reduce CLASS which executes when the entire process has completed
it will write an output file named success.txt with the tempVector in it
it will use IO management class to push the file to the output directory*/
void Reduce::exportSuccess()
{
    //write success and export it to an output file
    //after the entire input vector has been reduced and outputed.

    std::string fileName = "success.txt";
    std::vector<std::string> tempVector;
    tempVector.push_back("");
    IO_management.writeVectorToFile(outputDirectory, fileName, tempVector);
    return;
}





