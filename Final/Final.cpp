//The Final class contains the main fuctions for combining the results from the Reduce class, and merging them into one output file

#include "..\Final\Final.h"
#include "FileIO.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <Windows.h>
#include <filesystem>
#include <string>
#include <vector>
using std::string;
using std::vector;
using namespace std; 
namespace fs = std::filesystem;


//setting buffer
Final::Final()
    :   bufferLimit(1024) {
    }

//allocating buffer memory
Final::Final(std::string FinalDir)
    :   bufferLimit(1024),  finalOutputDirectory(FinalDir)
    {}

Final::Final(const string tMemory, size_t size_of_buffer)
    : intermediateDirectory{tMemory }, bufferLimit{ size_of_buffer }
    {}

//destructor
Final::~Final() 
    {};


/*Sets up the FinalOutput Directory, where the a single merged output file and a success file will written to*/
void Final::setUpFinalDirectory(std::string FinalDirIn)
{
    finalOutputDirectory = FinalDirIn;
}


/*Function getFileList is used to verify if any files exist in the Reducer Directory
if there are files, the bool listOfFiles will be set to true, else false*/
bool Final::getFileList(const std::string& inputFolder, std::vector<std::string>& fileList) {
    bool listOfFiles == true;
    
    if (IO_MAN.getListOfTextFiles(inputFolder, fileList)) {
        bool listOfFiles == true;
    }

    else {
        cout << "No files exist in the Reducer Directory" << endl;
        bool listOfFiles == false;
    }

    return listOfFiles;
}


/*the function importFromReduce is used for merging data from multiple files into one, the data is obtained from the
Reducer Directory, and imported into the Final
*/
bool Final::mergeFromReduce(const std::string& folderPath, const std::string& fileName, std::string& finalOutputVec)
{
    //import data from multiple files into a vector
    //have a while loop that says if any files are present in output directory then continue to process
    //if not then cout << No more files remain for extraction << endl;
    finalOutputVec = finalOutputFileName; 

    //empty vector declared
    std::vector<std::string> finVec;
    bool dataImportedSuccess == true; 
    bool listOfFiles;

    if (listOfFiles == true) {
            
        //read contents from file into finVec
        if (IO_MAN.readFileIntoVector(folderPath, fileName, finVec))
            {
                for (size_t i = 0; i < finVec.size(); i++) {
                    
                    if (listOfFiles == false) {
                        cout << "ERROR: Merge Unsuccessful at line " << finVec.at(i) << endl;
                    }
                } 
                //exSuccess(); //print out Success after merge is complete
            }

        else {
            cout << "ERROR: Issue with transfering data from reducer to Finalier" << endl;
            dataImportedSuccess == false; //not successful in transfering data
        }

        //exSuccess();
    }

    exSuccess(); //print out Success after merge is complete
    return dataImportedSuccess;
}    
        

/*
void Final::finalFile(const string& folderpath, const string& filename, std::string& finalOutputFile) 
{
}
*/

/*export a single output file with all data merged*/
bool Final::exportFinal(const std::string& fileName, std::vector<std::string> finVec)
{
     IO_MAN.writeVectorToFile(finalOutputDirectory, fileName, finVec, true);
     return true;
}

bool Final::finalFile(const std::string line) {

    bool dataImportedSuccess;
    std::vector<string> finVec;
    if (dataImportedSuccess == true) {
        
        exportFinal(finalOutputFileName, finVec);

    }

    else {

        cout << "ERROR: failure to package and export the merged file" << endl;
    }

    return;
}


/*export an output file with success written in it.*/
void Final::exSuccess()
{
    //writing success to an output file SUCCESS.TXT
    //After the completion of joining the results in finalFile

        std::string fName = "SUCCESS.txt";
        std::vector<std::string> vSucc;
        vSucc.push_back("SUCESSS!");
        IO_MAN.writeVectorToFile(finalOutputDirectory, fName, vSucc);
        return;
}
