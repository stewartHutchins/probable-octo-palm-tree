#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;

static const string recallDateStr="      \"recall_initiation_date\": \"";
static const string unitStr= "      \"units\": \"";
static const string caseStr= "      \"cases\": \"";


static const string dataFileName= "./../food-enforcement_UNITS_Inc_RR.json";
static const string outFileName= "./../RECALLS_OVER_TIME.csv";
static const string outFile2Name= "./../Units Vs Case.csv";

static const int startYr = 2011;
static const int endYr = 2017;

// function declaration
bool isUnitLine(string* line);
bool isCaseLine(string* line);
bool isRecallDateLine(string* line);
bool compareVariable(string* line, const string varName);

int main()
{
    ifstream foodFile(dataFileName);
    ofstream outFile(outFileName);
    ofstream outFile2(outFile2Name);


    if(!foodFile.is_open())
    {
        throw runtime_error("Error opening foodFile");
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Error opening outFile");
    }else if(!outFile2.is_open())
    {
        throw runtime_error("Error opening outFile2");
    }

    outFile << "Month,Number of Events,Number of Units,Number of Cases"<<endl;

    vector<string> dateList;
    vector<int> eventCountList;
    vector<int> unitCountList;
    vector<int> caseCountList;

    vector<int> unitVsCaseCountList;
    vector<int> caseVsUnitCountList;

    stringstream ss;
    ss << "Prior to jan " << startYr;
    dateList.push_back(ss.str());
    ss.str("");
    eventCountList.push_back(0);
    unitCountList.push_back(0);
    caseCountList.push_back(0);
    for(int i= startYr; i<=endYr; ++i)
    {
        for(int j=1; j<=12; ++j )
        {
            ss << i << "/" << j;
            dateList.push_back(ss.str());
            ss.str("");
            eventCountList.push_back(0);
            unitCountList.push_back(0);
            caseCountList.push_back(0);
        }
    }
    ss << "After Dec " << endYr;
    dateList.push_back(ss.str());
    ss.str("");
    eventCountList.push_back(0);
    unitCountList.push_back(0);
    caseCountList.push_back(0);

    for(unsigned i =0; i<dateList.size(); ++i)
    {
        cout << dateList.at(i) << endl;
    }

    string line;
    while(getline(foodFile, line))
    {
        if(isRecallDateLine(&line))
        {
            string date = line.substr(recallDateStr.length(), line.length()-recallDateStr.length()-2);
            //cout <<"\t\t" <<date <<endl;
            int i2 =(int)(date.at(2)) - (int)('0');
            int i3 =(int)(date.at(3)) - (int)('0');
            int yr = 2000 + (i2*10) + i3;
            int month;
            if(date.at(4)=='0')
            {
                month = (int)(date.at(5)) - (int)('0');
            }
            else
            {
                month =10+ (int)(date.at(5)) - (int)('0');
            }
            int ind;
            if(yr<startYr)
            {
                ind = 0;
            }
            else if(yr>endYr)
            {
                ind = (dateList.size()-1);
            }
            else
            {
                ind =((yr-startYr)*12) + month;
              //  cout <<"YR\t=\t"<<yr <<"/"<< month<<endl;
            }
            ++eventCountList.at(ind);
            int unit=0;
            while(getline(foodFile, line))
            {
                int temp=0;
                if(isUnitLine(&line))
                {
                    cout << "Adding to Unit Count" <<endl;
                    stringstream ss(line.substr(unitStr.length(), line.length()-unitStr.length()-2));
                    ss >> temp;
                    //cout << ss.str() <<endl;
                    if(!ss.fail())
                    {
                        (unitCountList.at(ind)) +=temp;
                        unit =temp;
                    }
                }
                else if(isCaseLine(&line))
                {
                    cout << "Adding to Case Count" <<endl;
                    stringstream ss(line.substr(caseStr.length(), line.length()-caseStr.length()-2));
                    ss >> temp;
                    if(!ss.fail())
                    {
                        (caseCountList.at(ind)) +=temp;
                        if(unit!=0 && temp!=0){
                            outFile2 << unit << "," << temp << "\n";
                        }
                    }
                    break;
                }
            }
        }
    }
    foodFile.close();
    outFile2.close();

    for(unsigned i=0; i<dateList.size(); ++i)
    {
        outFile << dateList.at(i) << ",";
        if(eventCountList.at(i) !=0)
        {
            outFile << eventCountList.at(i);
        }
        outFile << ",";
        if(unitCountList.at(i) !=0)
        {
            outFile << unitCountList.at(i);
        }
        outFile << ",";
        if(caseCountList.at(i) !=0)
        {
            outFile << caseCountList.at(i);
        }
        outFile<< "\n";
    }

    outFile.close();

    cout << "END" << endl;
    return 0;
}

bool compareVariable(string* line, const string varName)
{
    return (line->substr(0, varName.length())==varName);
}

bool isRecallDateLine(string* line)
{
    return compareVariable(line, recallDateStr);
}


bool isUnitLine(string* line)
{
    return compareVariable(line, unitStr);
}

bool isCaseLine(string* line)
{
    return compareVariable(line, caseStr);
}
