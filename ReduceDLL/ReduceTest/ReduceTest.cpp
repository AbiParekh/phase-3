// ReduceTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "../ReduceLib/ReduceLib.h"
#include <iostream>

void ReportResults(std::map<std::string, bool> outputMap);
bool Test_setParameters();
bool Test_IsolateWord();




class ReducerTestFramework : public Reduce
{
public:
	ReducerTestFramework() {}

	bool isolateWordFramework(const std::string& formattedWord, const std::string& startString, const std::string& endString, std::string& isloatedWord)
	{
		bool result = IsolateWord(formattedWord, startString, endString, isloatedWord);
		return result;
	}


	bool AddPhraseToMapFramework(const std::string& phrase, const std::string& startString, const std::string& endString)
	{
		return AddPhraseToMap(phrase, startString, endString);
	}

};



int main(int argc, char* argv[])
{
	std::map<std::string, bool>  testResults;

	// readFileIntoVector Test
	std::pair<std::string, bool> setParams("Test_setParameters", Test_setParameters());
	testResults.insert(setParams);


	// validDirectory Test   
	std::pair<std::string, bool> IsolateWord("Test_IsolateWord", Test_IsolateWord());
	testResults.insert(IsolateWord);


	ReportResults(testResults);

	return 0;
}

void ReportResults(std::map<std::string, bool> outputMap)
{
	std::cout << "\n\n\n\n\n" << std::endl;
	std::cout << "========================================== Test Results ==========================================" << std::endl;
	bool results = true;
	for (std::map<std::string, bool>::iterator outputMapIt = outputMap.begin(); outputMapIt != outputMap.end(); outputMapIt++)
	{
		std::string resultsformatted = "";
		if (outputMapIt->second == 0) resultsformatted = "FAIL";
		else  resultsformatted = "PASS";
		std::cout << "Test: " << outputMapIt->first << "; Result: " << resultsformatted << std::endl;
	}
	std::cout << "==================================================================================================" << std::endl;
}

bool Test_setParameters()
{
	Reduce temp;
	bool results = true;
	std::string first = "first";
	std::string second = "second";
	std::string checkerf, checkers;
	temp.setParameters(first, second);
	temp.getParameters(checkerf, checkers);
	if (first != checkerf) results = false;
	if (second != checkers) results = false;
	return results;
}

bool Test_IsolateWord()
{

	bool results_IsolateWord = true;
	std::string delim = "\"";
	std::string formattedWord = "(\"row\", 1)";
	std::string formattedWord2 = "(\"depth_charge\", 1)";
	std::string formattedWord3 = "NO Deliminator";
	std::string isloatedWord = "";
	std::string isloatedWord2 = "";
	std::string isloatedWord3 = "";
	ReducerTestFramework reduce;
	bool results = reduce.isolateWordFramework(formattedWord, delim, delim, isloatedWord);

	int correctWord = isloatedWord.compare("row");
	if (correctWord != 0 || results == false)
	{
		std::cout << "Test_IsolateWord: Unable to Isolate row" << std::endl;
		results_IsolateWord = false;
	}


	results = reduce.isolateWordFramework(formattedWord2, delim, delim, isloatedWord2);
	correctWord = isloatedWord2.compare("depth_charge");
	if (correctWord != 0 || results == false)
	{
		std::cout << "Test_IsolateWord: Unable to Isolate depth_charge" << std::endl;
		results_IsolateWord = false;
	}

	results = reduce.isolateWordFramework(formattedWord3, delim, delim, isloatedWord3);
	if (results != false)
	{
		std::cout << "Test_IsolateWord: did not fail when deliminator is missing" << std::endl;
		results_IsolateWord = false;
	}

	return results_IsolateWord;
}

