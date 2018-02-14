#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

static const string inFileName= "./../food-enforcement_FOODTYPE_AND_REASON_RR_UNITS.json";
static const string outFileName= "./../food-enforcement_DATE_CORRECTED_FOODTYPE_AND_REASON_RR_UNITS.json";
static const string recallInitDateStr= "      \"recall_initiation_date\": \"";

//function declaration
bool isDateStr(string* line);
bool compareVariable(string* line, const string varName);


int main()
{
    ifstream inFile(inFileName);
    ofstream outFile(outFileName);

    string line;
    while(getline(inFile, line))
    {
        if(isDateStr(&line))
        {
            cout << line <<endl;
            line = line.substr(0, recallInitDateStr.length()) +
                            line.substr(recallInitDateStr.length()+4+2, 2) + "/" +
                            line.substr(recallInitDateStr.length()+4, 2) + "/" +
                            line.substr(recallInitDateStr.length(), 4) + "\",";
            cout << line <<endl;
        }
        outFile << line << endl;
    }

    cout << "END" << endl;
    return 0;
}

bool compareVariable(string* line, const string varName)
{
    return (line->substr(0, varName.length())==varName);
}

bool isDateStr(string* line)
{
    return compareVariable(line, recallInitDateStr);
}

