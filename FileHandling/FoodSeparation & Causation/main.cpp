#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string foodTypesFileName= "./Food Types.txt";
static const string outFileName= "./FoodTypeVSCause.txt";
static const string outFileNameSub= "./FoodTypeVSCause.txt";
static const string topLevelFileName= "./Generalised totals.txt";
static const string unknownReasonsFileName= "./unknownReasons.txt";
static const string unknownFoodTypeFileName= "./unknownFood.txt";
static const string commonWordsFileName= "./commonWords.txt";


static const string recallReasonStr="      \"reason_for_recall\": \"";
static const string productDescStr="      \"product_description\": \"";
static const string reportDateStr="      \"report_date\": \"";
static const string recallingFirmStr="      \"recalling_firm\": \"";
static const string address2Str="      \"address_2\": \"";

static const string reasons[] ={"Undeclared Allergens","Mould/Spoiled","Salmonella","Machine Fragments/Glass","Other Contamination","Unknown"};
static const string alergens[] = {" almond"," walnut"," pecan"," cashew"," pistachio"," peanut","nut"," soy"," egg"," wheat"," dye"," shellfish"," fish", " milk "," dairy "};
static const string metalGlassCloth[] ={" metal"," glass"," cloth", "plastic"};

static const string alergenKeyWords[] ={" undeclared "," allergen"," label"};
static const string mouldKeyWords[] ={" mould"," spoil"};
static const string salmonellaKeyWords[] ={" salmonella "," listeria "," monocytogenes ","coli "};
static const string industrialContaminationKeyWords[] ={" glass", " shard"," metal"," cloth"," piece"," wire"," equipment"," fragment"," plastic", " foreign mat"};
static const string otherContaminationKeyWords[] ={" contaminat"};

struct RecallInfo;

//function declaration
void cleanString(string* str);
void removeSpaces(string* str);
bool isRecallReasonStr(string* str);
bool isUndeclaredAlergen(string* str);
bool isVariableStr(string* line, string* variableName);
bool isRecallReason(string* str);
bool isProductDesc(string* str);
bool isReportDate(string* str);
string getReportDate(string* str);
bool isRecallingFirm(string* str);
string getRecallingFirm(string* str);
bool isAdress2(string* str);
bool isMainReason(string* line, const string reasons[], unsigned reasonsLength);
string findSubReason(string* line, const string reasons[], unsigned reasonsLength);
string findUndeclaredAlergen(string* str);
bool applyToRi(RecallInfo* ri, string* reason, string* subreason);


struct RecallInfo{

    RecallInfo(string p){
        product=p;
        for(unsigned i=0; i<sizeof(reasons)/sizeof(reasons[0]);++i){
            causes.push_back(reasons[i]);
            causeCount.push_back(0);
        }
        subCause.push_back(reasons[5]);
        subCount.push_back(0);
    }
    string product;
    vector<string> causes;
    vector<string> subCause;
    vector<unsigned> causeCount;
    vector<unsigned> subCount;
};


unsigned unknownElementNo;

int main()
{
    unknownElementNo = sizeof(reasons)/sizeof(reasons[0])-1;

    ifstream dataFile(dataFileName);
    ifstream typeFile(foodTypesFileName);
    ofstream unknownReasonsFile(unknownReasonsFileName);
    ofstream unknownFoodTypeFile(unknownFoodTypeFileName);


    if(!dataFile.is_open() ){
        throw runtime_error("Error opening dataFile");
    }else if(!typeFile.is_open()){
        throw runtime_error("Error opening typeFile");
    }else if(!unknownReasonsFile.is_open()){
        throw runtime_error("Error opening unknown reasons file");
    }else if(!unknownFoodTypeFile.is_open()){
        throw runtime_error("Error opening unknown food type file");
    }

    vector<vector<RecallInfo>> foodTypes;
    string line;
    //////////////////////////////////////////read in all food types//////////////////////////////////////////////////////
    while(getline(typeFile, line)){
        if(line.substr(0,2) =="//"){
            //do nothing
        }else{
            vector<RecallInfo> vec;
            int ind =0;
            for(unsigned i=0; i<line.length(); ++i){
                if(line.at(i)==','){
                    string temp=line.substr(ind,i-ind);
                    vec.push_back(RecallInfo(temp));
                    ind=++i;
                }
            }
            foodTypes.push_back(vec);
        }
    }
    typeFile.close();

    //////////////////////////////////////////print out all food types//////////////////////////////////////////////////////
    for(unsigned i =0; i<foodTypes.size(); ++i){
        for(unsigned j=0; j<foodTypes.at(i).size(); ++j){
            cout << foodTypes.at(i).at(j).product <<endl;
        }
    }
    cout << endl << endl;

    //reason for recall
    string reason;

    //sub reason gives a more detailed reason for recall if nessisary, e.g. Reason=Undeclared allergen, subreason=milk
    string subreason;

    string reportDate;
    string recallingFirm;

    string tempReportDate;
    string tempRecallingFirm;
    unsigned totalRecalls =0;
    unsigned skipped =0;

    //////////////////////////////////////////read data file//////////////////////////////////////////////////////
    while(getline(dataFile, line)){
        if(isReportDate(&line)){
            tempReportDate=getReportDate(&line);
        }else if(isRecallingFirm(&line)){
            tempRecallingFirm=getRecallingFirm(&line);
            if(reportDate==tempReportDate && tempRecallingFirm==recallingFirm){
                //if event is duplicate, skip to just before product quantity
                ++skipped;
                while(getline(dataFile, line)){
                    if(isAdress2(&line)){
                        break;
                    }
                }
            }else{
                reportDate =tempReportDate;
                recallingFirm =tempRecallingFirm;
                ++totalRecalls;
            }

//////////////////////////////////////////analyse recall reason//////////////////////////////////////////////////////
       }else if(isRecallReason(&line)){
            cleanString(&line);
            line+=(' ');
            if(isMainReason(&line, alergenKeyWords, sizeof(alergenKeyWords)/sizeof(alergenKeyWords[0]))){
                reason = reasons[0];
                subreason = findUndeclaredAlergen(&line);
            }else if(isMainReason(&line, mouldKeyWords, sizeof(mouldKeyWords)/sizeof(mouldKeyWords[0]))){
                reason = reasons[1];
                subreason = reasons[1];
            }else if(isMainReason(&line, salmonellaKeyWords, sizeof(salmonellaKeyWords)/sizeof(salmonellaKeyWords[0]))){
                reason = reasons[2];
                subreason = reasons[2];
            }else if(isMainReason(&line, industrialContaminationKeyWords, sizeof(industrialContaminationKeyWords)/sizeof(industrialContaminationKeyWords[0]))){
                reason =  reasons[3];
                string temp =findSubReason(&line, metalGlassCloth, sizeof(metalGlassCloth)/sizeof(metalGlassCloth[0]));
                if(temp.empty()){
                    temp = reasons[unknownElementNo];
                }
                subreason = temp;
            }else if(isMainReason(&line, otherContaminationKeyWords, sizeof(otherContaminationKeyWords)/sizeof(otherContaminationKeyWords[0]))){
                reason = reasons[4];
                subreason = reasons[4];
            }else{
                //reason is unknown
                reason = reasons[unknownElementNo];
                subreason = reasons[unknownElementNo];
                unknownReasonsFile << line << "\n";
            }

//////////////////////////////////////////analyse product type//////////////////////////////////////////////////////

        }else if(isProductDesc(&line)){
            cleanString(&line);
            line+=(' ');
            bool added = false;
            //find food type
            RecallInfo* ri;
            for(unsigned i=0; i<foodTypes.size(); ++i){
                for(unsigned j=0; j<foodTypes.at(i).size(); ++j){

                    ri =&foodTypes.at(i).at(j);

                    //if correct food
                    if(line.find(ri->product) != string::npos ){
                        added = applyToRi(ri, &reason, &subreason);
                    }
                    if(added){
                        break;
                    }
                }
                if(added){
                    break;
                }
            }
            if(!added){
                //if not added, add it's because the pruduct is "Unknown/Other"
                applyToRi(ri, &reason, &subreason);
                unknownFoodTypeFile << line;
            }
        }
    }

    dataFile.close();
    unknownReasonsFile.close();

    //remove the spaces added earlier
    for(unsigned i=0; i<foodTypes.size(); ++i){
        for(unsigned j=0; j<foodTypes.at(i).size(); ++j){
                removeSpaces(&foodTypes.at(i).at(j).product);
        }
    }

//////////////////////////////////////////output quantity information//////////////////////////////////////////////////////

    ofstream outputFile(outFileName);
    ofstream outputFileSubCause(outFileNameSub);
    unsigned overallTotal[sizeof(reasons)/sizeof(reasons[0])];
    for(unsigned i=0; i<sizeof(overallTotal)/sizeof(overallTotal[0]); ++i){
        overallTotal[i] =0;
    }
    for(unsigned i=0; i<foodTypes.size(); ++i){
        for(unsigned j=0; j<foodTypes.at(i).size(); ++j){

            RecallInfo* ri = &foodTypes.at(i).at(j);

            outputFile << ri->product << ",";
            outputFileSubCause << ri->product << ",";
            for(unsigned k=0; k<ri->causes.size(); ++k){
                outputFile << ri->causes.at(k)<<"," << ri->causeCount.at(k) << ",";
                cout << ri->causes.at(k)<<"," << ri->causeCount.at(k) << ",";
                overallTotal[k] += (ri->causeCount.at(k));
            }
            for(unsigned k=0; k<ri->subCause.size(); ++k){
                outputFileSubCause << ri->subCause.at(k)<<"," << ri->subCount.at(k) << ",";
            }
            outputFile << "\n";
            outputFileSubCause << "\n";
        }
    }
    outputFile.close();
    outputFileSubCause.close();
    ofstream overallTotalFile(topLevelFileName);
    for(unsigned i=0; i<sizeof(reasons)/sizeof(reasons[0]); ++i){
        overallTotalFile << reasons[i] << "," << overallTotal[i] << "\n";
    }
    overallTotalFile.close();

    cout << "Total No# of Recalls:" <<endl << totalRecalls <<endl;
    cout << "Total No# skipped:" <<endl << skipped <<endl;



//////////////////////////////////////////analyse unknown types//////////////////////////////////////////////////////

    cout << "Analysing unknowns" <<endl;

    ifstream commonWordsFile(commonWordsFileName);
    vector<string> commonWords;
    while(commonWordsFile >> line){
        commonWords.push_back(line);
    }

    cout <<endl << "Analysing unknown reasons" <<endl;

    vector<string> unknownReasons;
    vector<unsigned> unknownReasonsCount;

    ifstream unknownReasonsFileIn(unknownReasonsFileName);
    while(unknownReasonsFileIn >> line){
        bool common =false;
        for(unsigned i=0; i<commonWords.size(); ++i){
            if(commonWords.at(i) == line){
                common=true;
            }
        }
        if(!common){
            bool added = false;
            for(unsigned i=0; i<unknownReasons.size(); ++i){
                if(unknownReasons.at(i)==line){
                    ++unknownReasonsCount.at(i);
                    added=true;
                    break;
                }
            }
            if(!added){
                unknownReasons.push_back(line);
                unknownReasonsCount.push_back(1);
            }
        }
    }
    for(unsigned i=0; i<unknownReasons.size(); ++i){
        if(unknownReasonsCount.at(i) >=100){
            cout <<unknownReasons.at(i) << "\t" <<unknownReasonsCount.at(i) <<"\n";
        }
    }

    cout <<endl << "Analysing unknown food types" <<endl;

    vector<string> unknownFoodTypes;
    vector<unsigned> unknownFoodTypesCount;
    ifstream unknownFoodTypesFileIn(unknownFoodTypeFileName);
    while(unknownFoodTypesFileIn >> line){
        bool common =false;
        for(unsigned i=0; i<commonWords.size(); ++i){
            if(commonWords.at(i) == line){
                common=true;
            }
        }
        if(!common){
            bool added = false;
            for(unsigned i=0; i<unknownFoodTypes.size(); ++i){
                if(unknownFoodTypes.at(i)==line){
                    ++unknownFoodTypesCount.at(i);
                    added=true;
                    break;
                }
            }
            if(!added){
                unknownFoodTypes.push_back(line);
                unknownFoodTypesCount.push_back(1);
            }
        }
    }
    for(unsigned i=0; i<unknownFoodTypes.size(); ++i){
        if(unknownFoodTypesCount.at(i)>=100){
            cout <<unknownFoodTypes.at(i) << "\t" <<unknownFoodTypesCount.at(i) <<"\n";
        }
    }

    cout << "END" << endl;
    return 0;
}

bool applyToRi(RecallInfo* ri, string* reason, string* subreason){
    cout << *reason << "," << *subreason<<endl;
    for(unsigned k=0; k<ri->causes.size(); ++k){
        cout << ".";
        if((ri->causes.at(k))==*reason){
            ++(ri->causeCount.at(k));
            break;
        }
    }
    for(unsigned k=0; k<ri->subCause.size(); ++k){
        cout << ".";
        if(ri->subCause.at(k) == *subreason){
            ++(ri->subCount.at(k));
            return true;
        }
    }
    ri->subCause.push_back(*subreason);
    ri->subCount.push_back(1);
    return true;
}


bool isVariableStr(string* line, const string variableName ){
    return ( (line->substr(0,variableName.length()))==variableName);
}

bool isRecallReason(string* str){
    return isVariableStr(str, recallReasonStr);
}

bool isProductDesc(string* str){
    return isVariableStr(str, productDescStr);
}

bool isReportDate(string* str){
    return isVariableStr(str, reportDateStr);
}

string getReportDate(string* str){
    return str->substr(reportDateStr.length(), 8);
}

bool isRecallingFirm(string* str){
    return isVariableStr(str, recallingFirmStr);
}

string getRecallingFirm(string* str){
    return str->substr(recallingFirmStr.length(), str->length()-2);
}

bool isAdress2(string* str){
    return isVariableStr(str, address2Str);
}
bool isMainReason(string* line, const string reasons[], unsigned reasonsLength){
    for(unsigned i=0; i<reasonsLength; ++i){

        if(line->find((reasons)[i]) != string::npos){
            return true;
        }
    }

    return false;
}

string findSubReason(string* line, const string subReasons[], unsigned reasonsLength){
    for(unsigned i=0; i<sizeof(subReasons)/sizeof(subReasons[0]); ++i){
        if(line->find((subReasons)[i]) != string::npos){
            return subReasons[i];
        }
    }
    return reasons[unknownElementNo];
}

string findUndeclaredAlergen(string* str){

    vector<unsigned> inds;
    //look up the start and end positions of "undeclared" and "alergen" (and any other added later)
    for(unsigned i=0; i<sizeof(alergenKeyWords)/sizeof(alergenKeyWords[0]); ++i){
        if(str->find(alergenKeyWords[i]) !=string::npos){
            inds.push_back(str->find(alergenKeyWords[i]));
            inds.push_back( (str->find(alergenKeyWords[i])) + alergenKeyWords[i].length());
        }
    }

    int mi=str->length();
    int ma=0;
    for(unsigned i=0; i<inds.size(); ++i ){
        if(inds[i]<mi){
            mi = inds[i];
        }else if(inds[i]>ma){
            ma = inds[i];
        }
    }


    string undeclaredAlergen="";
    bool found=false;
    //while not found slowly increment the length of the string
    string sub;

    while(!found){

        sub = str->substr(mi, ma-mi);
        mi-=5;
        ma+=5;
        if(mi<0){
            mi=0;
        }
        if(ma>str->length()-1){
            ma=str->length()-1;
        }

        for(int i=0; i<(sizeof(alergens)/sizeof(alergens[0])); ++i){
            if(sub.find(alergens[i]) !=string::npos){
                undeclaredAlergen = alergens[i];
                found =true;
            }
        }
        //if the string being looked at spans the entire line, and we haven't found the alergen, set alergen to unknown
        if(mi==0 && ma==(str->length()-1) && undeclaredAlergen.empty()){
            undeclaredAlergen = reasons[unknownElementNo];
            found=true;
        }
    }
    return undeclaredAlergen;
}

void removeSpaces(string* str){
     for(string::iterator i = str->begin(); i != str->end(); i++)
    {
        if(str->at(i - str->begin()) == ' ')
        {
            str->erase(i);
            --i;
        }
    }
}

void cleanString(string* str){
     for(string::iterator i = str->begin(); i != str->end(); i++)
    {
        if(!isalpha(str->at(i - str->begin())) && str->at(i - str->begin()) != ' ')
        {
            str->erase(i);
            --i;
        }
    }

    transform(str->begin(), str->end(), str->begin(), ::tolower);
}

