#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement_FOODTYPE_AND_REASON_RR_UNITS.json";
static const string outFileName= "./../200sampleFOODTYPE_AND_REASON_RR_UNITS.json";

static double numberOfSamplesWanted = 200.0;

void toLowerCase(string* str );

int main()
{
    ifstream dataFile(dataFileName);
    ofstream outFile(outFileName);

    if(!dataFile.is_open() )
    {
        throw runtime_error("Error opening dataFile");
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Error opening outFile");
    }

    string line;
    int eventCount=0;
    while(getline(dataFile, line))
    {
        if(line == "    {")
        {
            ++eventCount;
        }
    }
    dataFile.close();
    dataFile.open(dataFileName);
    if(!outFile.is_open())
    {
        throw runtime_error("Error opening outFile, 2nd time");
    }

    outFile << "{" <<endl;
    double random;
    double probability;
    double checkCount =0;
    while(getline(dataFile, line))
    {
        probability = numberOfSamplesWanted / (double)eventCount;
        random = ((double)rand() / (RAND_MAX));
        if(line == "    {")
        {
            if(random <= probability)
            {
                outFile <<line <<endl;
                while(getline(dataFile, line))
                {
                    outFile << line <<endl;
                    if(line=="    },")
                    {
                        ++checkCount;
                        --numberOfSamplesWanted;
                        break;
                    }
                }
            }
            --eventCount;
        }
    }
    outFile << "  ]\n}";
    dataFile.close();
    outFile.close();
    cout << "Number of Samples: " << checkCount << endl;
    cout << "END" << endl;
    return 0;
}

void toLowerCase(string* str )
{
    transform(str->begin(), str->end(), str->begin(), ::tolower);
}
