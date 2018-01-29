#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;

static const string recallTime="      \"recall_initiation_date\": \"";
static const string unitStr= "      \"units\": \"";

static const string dataFileName= "./../food-enforcement_REPEATS_REMOVED.json";
static const string outFileName= "./../RECALLS_OVER_TIME_BY_EVENTS.csv";
static const string outFileUnitName= "./../RECALL_OCCOURANCES_OVER_TIME_BY_UNITS.csv";
static const string outFileCaseName= "./../RECALL_OCCOURANCES_OVER_TIME_BY_CASES.csv";


static const int startYr = 2011;
static const int endYr = 2017;


int main()
{
    ifstream foodFile(datafileName);
    ofstream outFile1(dateName);
    ofstream outFile2(dateNameIncFirm);

    if(!foodFile.is_open())
    {
        throw runtime_error("Error opening foodFile");
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Error opening out1File");
    }

    vector<string> dateList;
    vector<int> dateCount;
    vector<int> unitCount;


    stringstream ss;
    ss << "Prior to jan " << startYr;
    dateList.push_back(ss.str());
    ss.str("");
    dateCount.push_back(0);
    unitCount.push_back(0);

    for(int i= startYr; i<=endYr; ++i)
    {
        for(int j=1; j<=12; ++j )
        {
            ss << i << "/" << j;
            dateList.push_back(ss.str());
            ss.str("");
            dateCount.push_back(0);
            unitCount.push_back(0);

        }
    }
    ss << "After Dec " << endYr;
    dateList.push_back(ss.str());
    ss.str("");
    dateCount.push_back(0);
    unitCount.push_back(0);


    for(unsigned i =0; i<dateList.size(); ++i)
    {
        cout << dateList.at(i) << endl;
    }

    string line;
    while(getline(foodFile, line))
    {
        if(isRecallDateLine(line))
        {
            string date = line.substr(recallTime.length(), line.length()-recallTime.length()-2);
            cout <<"\t\t" <<date <<endl;
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
                ind = (dateCount.size()-1);
            }
            else
            {
                ind =((yr-startYr)*12) + month;
                cout <<"YR\t=\t"<<yr <<"/"<< month<<endl;
            }
            ++dateCount.at(ind);
            while(getline(foodFile, line))
            {
                if(isUnitLine(&line))
                {

                    break;
                }
                else if(isCaseLine(&line))
                {

                }
            }
            break;
        }
    }

    for(unsigned i=0; i<dateList.size(); ++i)
    {
        outFile1 << dateList.at(i) << "," << dateCount.at(i) << "\n";
        cout << dateList.at(i) << "\t" << dateCount.at(i) <<"\t\t|";
        for(unsigned j=0; j<recallingFirms.at(i).size(); ++j)
        {
            outFile2 <<"," << recallingFirms.at(i).at(j);
            cout <<"," << recallingFirms.at(i).at(j);
        }
        cout <<"\n";
        outFile2 << "\n";
    }

    foodFile.close();
    outFile1.close();
    outFile2.close();

    cout << "END" << endl;
    return 0;
}

bool compareVariable(string* line, const string varName)
{
    return (line->substr(0, varName.length())==varName);
}

bool isUnitLine(string* line)
{
    return compareVariable(line, recallTime);
}

bool isUnitLine(string* line)
{
    return compareVariable(line, unitString);
}
