#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement_UNITS_Inc_RR.json";
static const string foodTypesFileName= "./Food Types.txt";
static const string outFileName= "./../food-enforcement_FOODTYPE_AND_REASON_RR_UNITS.json";
static const string unknownReasonsFileName= "./unknownReasons.txt";
static const string unknownFoodTypeFileName= "./unknownFood.txt";
static const string commonWordsFileName= "./../commonWords.txt";


static const string recallReasonStr="      \"reason_for_recall\": \"";
static const string productDescStr="      \"product_description\": \"";
static const string resultsStr = "  \"results\": [";
static const string endStr1="    },";
static const string endStr2="    }";

static const string reasons[] = {   "Food Poisoning",
                                    "Pesticide",
                                    "Residue",
                                    "Mould/Spoiled",
                                    "GMP Violation",
                                    "Machine Fragments/Glass",
                                    "Undeclared Ingredients",
                                    "Other Contamination",
                                    "Unknown"};
static const string alergens[] = {"almond","walnut","pecan","cashew","pistachio","peanut","nut","soy","egg","wheat","dye","shellfish","fish","milk","dairy"};
static const string metalGlassCloth[] = {"metal","glass","cloth","plastic"};
static const string foodPoisoningTypes[] = {"salmonella","listeria","coli"};

static const string foodPoisoningKeyWords[] = {"salmonella ","listeria ","monocytogenes ","coli"};
static const string pesticideKeyWords[] = {"pesticide"};
static const string residueKeyWords[] = {"adulteration", "residue"};
static const string mouldKeyWords[] = {"mould","spoil","mold"};
static const string industrialContaminationKeyWords[] = {"glass","shard","metal","cloth","piece","wire","equipment","fragment","foreign object","foreign mat"};
static const string undeclaredKeyWords[] = {"declar","allergen","label", "may contain"};
static const string otherContaminationKeyWords[] = {"contaminat"};
static const string gmps[] = {"manufactured under gmp", "cgmp"};


//function declaration
string cleanString(string str);

bool compareVariable(string* line, string variableName);
bool isRecallReasonStr(string* str);
bool isUndeclaredAlergen(string* str);
bool isRecallReason(string* str);
bool isProductDesc(string* str);
bool isMainReason(string* line, const string reasons[], unsigned reasonsLength);
bool isEnd(string* line);

string findSubReason(string* line, const string reasons[], unsigned reasonsLength);
string findUndeclaredAlergenResidue(string* str, const string alergenResidue[], int len);

unsigned unknownElementNo;

int main()
{
    unknownElementNo = sizeof(reasons)/sizeof(reasons[0])-1;

    ifstream dataFile(dataFileName);
    ifstream typeFile(foodTypesFileName);
    ofstream unknownReasonsFile(unknownReasonsFileName);
    ofstream unknownFoodTypeFile(unknownFoodTypeFileName);
    ofstream outFile(outFileName);

    if(!dataFile.is_open() )
    {
        throw runtime_error("Error opening dataFile");
    }
    else if(!typeFile.is_open())
    {
        throw runtime_error("Error opening typeFile");
    }
    else if(!unknownReasonsFile.is_open())
    {
        throw runtime_error("Error opening unknown reasons file");
    }
    else if(!unknownFoodTypeFile.is_open())
    {
        throw runtime_error("Error opening unknown food type file");
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Error opening output file");
    }

    string line;
    //////////////////////////////////////////read in all food types//////////////////////////////////////////////////////
    vector<vector<string>> foodTypes;
    while(getline(typeFile, line))
    {
        if(line.substr(0,2) =="//")
        {
            //do nothing
        }
        else
        {
            vector<string> vec;
            int ind =0;
            for(unsigned i=0; i<line.length(); ++i)
            {
                if(line.at(i)==',')
                {
                    string temp=line.substr(ind,i-ind);
                    vec.push_back(temp);
                    ind=++i;
                }
            }
            foodTypes.push_back(vec);
        }
    }
    typeFile.close();

    //////////////////////////////////////////print out all food types//////////////////////////////////////////////////////
    for(unsigned i =0; i<foodTypes.size(); ++i)
    {
        for(unsigned j=0; j<foodTypes.at(i).size(); ++j)
        {
            cout << foodTypes.at(i).at(j) <<endl;
        }
    }
    cout << endl << endl;

    //reason for recall
    string reason;
    //sub reason gives a more detailed reason for recall if nessisary, e.g. Reason=Undeclared allergen, subreason=milk
    string subreason;
    //food a specific type
    string food;
    //a more generalized food type for analysis pourposes
    string generalFood;

    //////////////////////////////////////////read data file//////////////////////////////////////////////////////
    vector<string> lines;
    bool started = false;
    outFile << "{\n  \"meta\": {\n  },\n";
    int unknownReasonCount =0;
    int unknownFoodCount =0;

    while(getline(dataFile, line))
    {
        if(!started)
        {
            started = (line==resultsStr);
        }
        if(started)
        {

//////////////////////////////////////////analyse recall reason//////////////////////////////////////////////////////
            if(isRecallReason(&line))
            {
                string cleanedString = cleanString(line);
                cleanedString+=(' ');
                if(isMainReason(&cleanedString, foodPoisoningKeyWords, sizeof(foodPoisoningKeyWords)/sizeof(foodPoisoningKeyWords[0])))
                {
                    reason = reasons[0];
                    subreason = reasons[0] + "_" + findSubReason(&cleanedString, foodPoisoningTypes, sizeof(foodPoisoningTypes)/sizeof(foodPoisoningTypes[0]));
                }else if(isMainReason(&cleanedString, pesticideKeyWords, sizeof(pesticideKeyWords)/sizeof(pesticideKeyWords[0])))
                {
                    reason = reasons[1];
                    subreason = reasons[1];
                }else if(isMainReason(&cleanedString, residueKeyWords, sizeof(residueKeyWords)/sizeof(residueKeyWords[0]))){
                    reason = reasons[2];
                    subreason = reasons[2] + "_" + findUndeclaredAlergenResidue(&cleanedString, residueKeyWords, (sizeof(residueKeyWords)/sizeof(residueKeyWords[0])));
                }
                else if(isMainReason(&cleanedString, mouldKeyWords, sizeof(mouldKeyWords)/sizeof(mouldKeyWords[0])))
                {
                    reason = reasons[3];
                    subreason = reasons[3];
                }else if(isMainReason(&cleanedString, gmps, sizeof(gmps)/sizeof(gmps[0])))
                {
                    reason = reasons[4];
                    subreason = reasons[4];
                }else if(isMainReason(&cleanedString, industrialContaminationKeyWords, sizeof(industrialContaminationKeyWords)/sizeof(industrialContaminationKeyWords[0])))
                {
                    reason = reasons[5];
                    subreason = reasons[5] + "_" + findSubReason(&cleanedString, metalGlassCloth, sizeof(metalGlassCloth)/sizeof(metalGlassCloth[0]));
                }
                else if(isMainReason(&cleanedString, undeclaredKeyWords, sizeof(undeclaredKeyWords)/sizeof(undeclaredKeyWords[0])))
                {
                    reason = reasons[6];
                    subreason = reasons[6] + "_" + findUndeclaredAlergenResidue(&cleanedString, undeclaredKeyWords, sizeof(undeclaredKeyWords)/sizeof(undeclaredKeyWords[0]));
                }else if(isMainReason(&cleanedString, otherContaminationKeyWords, sizeof(otherContaminationKeyWords)/sizeof(otherContaminationKeyWords[0])))
                {
                    reason = reasons[7];
                    subreason = reasons[7];
                }else {
                    //reason is unknown
                    ++unknownReasonCount;
                    reason = reasons[unknownElementNo];
                    subreason = reasons[unknownElementNo] + "_" + reasons[unknownElementNo];
                    unknownReasonsFile << cleanedString << "\n";
                }

//////////////////////////////////////////analyse product type//////////////////////////////////////////////////////

            }
            else if(isProductDesc(&line))
            {
                string cleanedString = cleanString(line);
                cleanedString+=(' ');
                bool added = false;
                //find food type
                for(unsigned i=0; i<foodTypes.size(); ++i)
                {
                    for(unsigned j=0; j<foodTypes.at(i).size(); ++j)
                    {
                        //if correct food
                        if(cleanedString.find(foodTypes.at(i).at(j)) != string::npos )
                        {
                            food = foodTypes.at(i).at(j);
                            generalFood = foodTypes.at(i).at(foodTypes.at(i).size()-1);
                            added = true;
                        }
                        if(added)
                        {
                            break;
                        }
                    }
                    if(added)
                    {
                        break;
                    }
                }

                if(!added)
                {
                    //if not added, add it's because the product is "Unknown/Other"
                    ++unknownFoodCount;
                    food=reasons[unknownElementNo];
                    generalFood=reasons[unknownElementNo];
                    unknownFoodTypeFile << line << "\n";
                }
            }
            lines.push_back(line);

            if(isEnd(&line))
            {
                for(int i =0; i<lines.size()-2; ++i)
                {
                    outFile << lines.at(i) << "\n";
                }
                outFile << lines.at(lines.size()-2) << ",\n";
                outFile << "      \"general_food\": \"" << generalFood <<"\",\n";
                outFile << "      \"specific_food\": \"" << food <<"\",\n";
                outFile << "      \"reason\": \"" << reason <<"\",\n";
                outFile << "      \"specific_reason\": \"" << subreason <<"\"\n";
                outFile << lines.at(lines.size()-1) << "\n";
                lines.clear();
                cout << ".";
            }
        }
    }
    outFile << "  ]\n}";
    outFile.close();
    dataFile.close();
    unknownReasonsFile.close();

//////////////////////////////////////////analyse unknown types//////////////////////////////////////////////////////
    cout << endl << "No# of unknown food types: " << unknownFoodCount << endl;
    cout << "No# of unknown reasons: " << unknownReasonCount << endl << endl;


    cout << endl << "Analysing unknowns" <<endl;

    ifstream commonWordsFile(commonWordsFileName);
    if(!commonWordsFile.is_open()){
        throw runtime_error("Error opening commonWordsFile");
    }
    vector<string> commonWords;
    while(commonWordsFile >> line)
    {
        commonWords.push_back(line);
    }

    cout <<endl << "Analysing unknown reasons" <<endl;

    vector<string> unknownReasons;
    vector<unsigned> unknownReasonsCount;

    ifstream unknownReasonsFileIn(unknownReasonsFileName);
    while(unknownReasonsFileIn >> line)
    {
        bool common =false;
        for(unsigned i=0; i<commonWords.size(); ++i)
        {
            if(commonWords.at(i) == line)
            {
                common=true;
            }
        }
        if(!common)
        {
            bool added = false;
            for(unsigned i=0; i<unknownReasons.size(); ++i)
            {
                if(unknownReasons.at(i)==line)
                {
                    ++unknownReasonsCount.at(i);
                    added=true;
                    break;
                }
            }
            if(!added)
            {
                unknownReasons.push_back(line);
                unknownReasonsCount.push_back(1);
            }
        }
    }
    for(unsigned i=0; i<unknownReasons.size(); ++i)
    {
        if(unknownReasonsCount.at(i) >=20)
        {
            cout <<unknownReasons.at(i) << "\t" <<unknownReasonsCount.at(i) <<"\n";
        }
    }

    cout <<endl << "Analysing unknown food types" <<endl;

    vector<string> unknownFoodTypes;
    vector<unsigned> unknownFoodTypesCount;
    ifstream unknownFoodTypesFileIn(unknownFoodTypeFileName);
    while(unknownFoodTypesFileIn >> line)
    {
        bool common =false;
        for(unsigned i=0; i<commonWords.size(); ++i)
        {
            if(commonWords.at(i) == line)
            {
                common=true;
            }
        }
        if(!common)
        {
            bool added = false;
            for(unsigned i=0; i<unknownFoodTypes.size(); ++i)
            {
                if(unknownFoodTypes.at(i)==line)
                {
                    ++unknownFoodTypesCount.at(i);
                    added=true;
                    break;
                }
            }
            if(!added)
            {
                unknownFoodTypes.push_back(line);
                unknownFoodTypesCount.push_back(1);
            }
        }
    }
    for(unsigned i=0; i<unknownFoodTypes.size(); ++i)
    {
        if(unknownFoodTypesCount.at(i) >= 20)
        {
            cout <<unknownFoodTypes.at(i) << "\t" <<unknownFoodTypesCount.at(i) <<"\n";
        }
    }

    cout << "END" << endl;
    return 0;
}


bool compareVariable(string* line, const string variableName )
{
    return ( (line->substr(0,variableName.length()))==variableName);
}

bool isRecallReason(string* str)
{
    return compareVariable(str, recallReasonStr);
}

bool isProductDesc(string* str)
{
    return compareVariable(str, productDescStr);
}

bool isEnd(string* line)
{
    return (compareVariable(line, endStr1) || compareVariable(line, endStr2));
}


bool isMainReason(string* line, const string reasons[], unsigned reasonsLength)
{
    for(unsigned i=0; i<reasonsLength; ++i)
    {

        if(line->find((reasons)[i]) != string::npos)
        {
            return true;
        }
    }

    return false;
}

string findSubReason(string* line, const string subReasons[], unsigned reasonsLength)
{
    for(unsigned i=0; i<reasonsLength; ++i)
    {
        if(line->find((subReasons)[i]) != string::npos)
        {
            return subReasons[i];
        }
    }
    return reasons[unknownElementNo];
}

string findUndeclaredAlergenResidue(string* str, const string alergenResidue[], int len)
{

    vector<unsigned> inds;
    //look up the start and end positions of "undeclaredKeyWords" and "alergen" (and any other added later)
    for(unsigned i=0; i<len; ++i)
    {
        if(str->find((*alergenResidue)[i]) !=string::npos)
        {
            inds.push_back(str->find(alergenResidue[i]));
            inds.push_back( (str->find(alergenResidue[i])) + (alergenResidue)[i].size());
        }
    }

    int mi=str->length();
    int ma=0;
    for(unsigned i=0; i<inds.size(); ++i )
    {
        if(inds[i]<mi)
        {
            mi = inds[i];
        }
        else if(inds[i]>ma)
        {
            ma = inds[i];
        }
    }


    string undeclaredAlergen="";
    bool found=false;
    //while not found slowly increment the length of the string
    string sub;

    while(!found)
    {

        sub = str->substr(mi, ma-mi);
        mi-=5;
        ma+=5;
        if(mi<0)
        {
            mi=0;
        }
        if(ma>str->length()-1)
        {
            ma=str->length()-1;
        }

        for(int i=0; i<(sizeof(alergens)/sizeof(alergens[0])); ++i)
        {
            if(sub.find(alergens[i]) !=string::npos)
            {
                undeclaredAlergen = alergens[i];
                found =true;
            }
        }
        //if the string being looked at spans the entire line, and we haven't found the alergen, set alergen to unknown
        if(mi==0 && ma==(str->length()-1) && undeclaredAlergen.empty())
        {
            undeclaredAlergen = reasons[unknownElementNo];
            found=true;
        }
    }
    return undeclaredAlergen;
}


string cleanString(string str)
{
    for(string::iterator i = str.begin(); i != str.end(); i++)
    {
        if(!isalpha(str.at(i - str.begin())) && str.at(i - str.begin()) != ' ')
        {
            str.erase(i);
            --i;
        }
    }

    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

