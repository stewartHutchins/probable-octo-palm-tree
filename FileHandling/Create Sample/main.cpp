#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string outFileName= "./FoodTypeVSCause.txt";
static const string outFileNameSub= "./FoodTypeVSCause.txt";

static const string recallReasonStr="      \"reason_for_recall\": \"";
static const string productDescStr="      \"product_description\": \"";

//function declaration
void cleanWord(string* word);


int main()
{
    ifstream dataFile(dataFileName);

    if(!dataFile.is_open() ){
        throw runtime_error("Error opening dataFile");
    }

    while(getline(dataFile, line)){

        if(line.substr(0,recallReasonStr.length())==recallReasonStr){
            cleanWord(&line);

        }else if(line.substr(0,productDescStr.length())==productDescStr){
            cleanWord(&line);
        }
    }
                                 cout << "STUCK YOYOYOYOOY";

    dataFile.close();

    ofstream outputFile(outFileName);
    ofstream outputFileSubCause(outFileNameSub);
    int overallTotal[sizeof(reasons)/sizeof(reasons[0])];
    for(int i=0; i<foodTypes.size(); ++i){
        for(int j=0; j<foodTypes.at(i).size(); ++j){
            RecallInfo* ri = &foodTypes.at(i).at(j);
            outputFile << ri->product << ",";
            outputFileSubCause << ri->product << ",";
            for(int k=0; k<ri->causes.size(); ++k){
                outputFile << ri->causes.at(k)<<"," << ri->causeCount.at(k) << ",";
                cout << ri->causes.at(k)<<"," << ri->causeCount.at(k) << ",";
                overallTotal[k] += ri->causeCount.at(k);
            }
            for(int k=0; k<ri->subCause.size(); ++k){
                outputFileSubCause << ri->subCause.at(k)<<"," << ri->subCount.at(k) << ",";
            }
            outputFile << "\n";
            outputFileSubCause << "\n";
        }
    }
    outputFile.close();
    outputFileSubCause.close();
    ofstream overallTotalFile("./Generalised totals.txt");
    for(int i=0; i<sizeof(reasons)/sizeof(reasons[0]); ++i){
        overallTotalFile << reasons[i] << "," << overallTotal[i] << "\n";
    }
    overallTotalFile.close();

    cout << "END" << endl;
    return 0;
}

void cleanWord(string* word){
    for(string::iterator i = word->begin(); i != word->end(); i++)
    {
        if(!isalpha(word->at(i - word->begin())) && !(word->at(i - word->begin())==' '))
        {
            word->erase(i);
            --i;
        }
    }
    transform(word->begin(), word->end(), word->begin(), ::tolower);
}
