#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

//function declaration
bool compareVariable(string* line, const string varName);
bool isProductQuan(string* line);
bool isWeightUnit(string str);
bool isEnd(string* str);
string cleanStr(string line);
bool isUnitType(string str);
int stringToInt(string str);


static const string inFileName= "./../food-enforcement_REPEATS_REMOVED.json";
static const string outFileName= "./../food-enforcement_UNITS_Inc_RR.json";
static const string productQuanStr= "      \"product_quantity\": \"";

static const string weightTypes[] = {"oz", "lb", "pound", "gallon", "litre", "total", "pint"};
static const string unitTypes[] = {"units", "bottles", "cakes", "cookies","bars", "pies", "jars", "tins", "cans", "cartons","ct", "sandwiches", "tubs"};

int main()
{
    ifstream foodFile(inFileName);
    ofstream outFile(outFileName);
    if(!foodFile.is_open())
    {
        throw runtime_error("Error opening foodFile");
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Error opening outFile");
    }

    string line;
    int units;
    int cases;
    int boxes;
    int noUnitType=0;
    while(getline(foodFile, line))
    {
        if(isProductQuan(&line))
        {
            string cleanedLine = cleanStr(line.substr(productQuanStr.length(), line.length()-productQuanStr.length()-1));
            stringstream ss(cleanedLine);
            string quantity;
            string unitType;
            units=0;
            cases=0;
            boxes=0;
            while(ss >>unitType)
            {
                if(isUnitType(unitType))
                {
                    units +=stringToInt(quantity);
                }
                else if(unitType.find("case")!=string::npos ||unitType.find("cs")!=string::npos)
                {
                    cases +=stringToInt(quantity);
                }
                else if(unitType.find("box")!=string::npos)
                {
                    boxes +=stringToInt(quantity);
                }
                quantity = unitType;
            }
            if(units==0)    // occasionally an event will occur when the word 'units' is implied, but not stated
            {
                bool allNum=true;
                cleanedLine = cleanStr(line.substr(productQuanStr.length(), line.length()-productQuanStr.length()));
                for(string::iterator i = cleanedLine.begin(); i != cleanedLine.end(); ++i)
                {
                    if(!isdigit(cleanedLine.at(i - cleanedLine.begin())) && cleanedLine.at(i - cleanedLine.begin()) != ' ')
                    {
                        allNum=false;
                        break;
                    }
                }
                if(allNum)
                {
                    ss.str();       //empty string stream
                    ss.clear();     //reset any flags
                    ss << cleanedLine;
                    while(ss >> quantity)
                    {
                        units+=stringToInt(quantity);
                    }
                }
            }
        }
        else if(isEnd(&line))
        {
            if(units==0 && boxes==0 && cases ==0)
            {
                ++noUnitType;
            }

            outFile << "," <<endl;
            outFile << "      \"units\": \"";
            if(units!=0 && units > cases)
            {
                outFile << units;
            }
            outFile<< "\"," << endl;
            outFile << "      \"cases\": \"";
            if(cases!=0)
            {
                outFile << cases;
            }
            outFile <<"\"," << endl;
            outFile << "      \"boxes\": \"";
            if(boxes !=0)
            {
                outFile << boxes;
            }
            outFile << "\"" << endl;
        }
        outFile << line;
        if(!isProductQuan(&line))
        {
            outFile << endl;
        }
    }

    foodFile.close();
    outFile.close();

    cout <<"There were " << noUnitType << " instances where no unit type could be derived" << endl;
    cout << "END" << endl;
    return 0;
}


bool compareVariable(string* line, const string varName)
{
    return (line->substr(0, varName.length())==varName);
}

bool isProductQuan(string* line)
{
    return compareVariable(line, productQuanStr);
}

bool isEnd(string* line)
{
    return (*line=="    }," || *line=="    }" );
}


int stringToInt(string str)
{
    cout << str <<endl;
    stringstream ss(str);
    int i;
    ss >> i;
    if(ss.fail())
    {
        return 0;
    }
    else
    {
        return i;
    }
}

bool isWeightUnit(string str)
{
    for(unsigned i=0; i<sizeof(weightTypes)/sizeof(weightTypes[0]); ++i)
    {
        if(str.find(weightTypes[i]) !=string::npos)
        {
            return true;
        }
    }
    return false;
}

bool isUnitType(string str)
{
    for(unsigned i=0; i<sizeof(unitTypes)/sizeof(unitTypes[0]); ++i)
    {
        if(str.find(unitTypes[i]) !=string::npos)
        {
            return true;
        }
    }
    return false;
}


string cleanStr(string str)
{
    for(string::iterator i = str.begin(); i != str.end(); i++)
    {
        //if(!isalnum(str.at(i - str.begin())) && str.at(i - str.begin()) != ' ')
        if(str.at(i-str.begin()) == ',')
        {
            str.erase(i);
            --i;
        }
    }
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}


