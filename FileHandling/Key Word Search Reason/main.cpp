#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string outFileName= "./keyWordsReason.csv";
static const string commonFileName= "./commonWords.txt";


static const string recallReasonStr="      \"reason_for_recall\": \"";
static const string recallingFirmStr = "      \"recalling_firm\": \"";
static const string recallInitDate= "      \"recall_initiation_date\": \"";
static const string state="      \"state\": \"";
static const string firmNotificationStr= "initialfirmnotification";

//function declaration
void cleanWord(string* word);

struct BusinessDate{

    BusinessDate(string rf, string d){
        recallingFirm=rf;
        date=d;
    }
    string date;
    string recallingFirm;
};

int main()
{
    ifstream foodFile(dataFileName);
    ofstream outFile(outFileName);
    ifstream commonFile(commonFileName);

    if(!foodFile.is_open() ){
        throw runtime_error("Error opening foodFile");
    }else if(!outFile.is_open()){
        throw runtime_error("Error opening outFile");
    }else if(!commonFile.is_open()){
        throw runtime_error("Error opening commonFile");
    }

    vector<string> commonList;
    string word;
    while(commonFile >> word){
        commonList.push_back(word);
    }
    commonFile.close();

    vector<string> wordList;
    vector<int> wordCount;
    vector<BusinessDate> businessDateList;

    unsigned totalWords = 0;

    string line;
    string tempFirmName;
    string date;
    bool repeat;
    while(getline(foodFile, line)){
        if(line.substr(0,recallInitDate.length())==recallInitDate){
            repeat=false;
            date=line.substr(recallInitDate.length(), line.length()-recallInitDate.length()-2);
        }else if(line.substr(0,recallingFirmStr.length())==recallingFirmStr){
            tempFirmName=line.substr(recallingFirmStr.length(), line.length()-recallingFirmStr.length()-2);
            if(!businessDateList.empty()){
                for(int i =(businessDateList.size()-1); i>=0; --i){
                    BusinessDate* tempBusinessDate = &(businessDateList.at(i));
                    if(tempBusinessDate->date== date && tempBusinessDate->recallingFirm==tempFirmName){
                        repeat=true;
                        break;
                    }
                }
            }
        }else if(line.substr(0,state.length())==state && !repeat){
            //get rid of var name
            foodFile >> line;
            businessDateList.push_back( BusinessDate(tempFirmName, date));
            while(foodFile >> word){
                cleanWord(&word);
                if(word==firmNotificationStr){
                    break;
                }
                bool add = true;
                for(unsigned i=0; i<commonList.size(); ++i){
                    if(word==commonList.at(i)){
                         add = false;
                    }
                }
                if(add){
                   for(unsigned i=0; i<wordList.size(); ++i){
                        if(word==wordList.at(i)){
                            ++wordCount.at(i);
                            add= false;
                            break;
                        }
                    }
                    if(add){
                        wordList.push_back(word);
                        wordCount.push_back(1);
                    }
                }
            }
        }
    }
    bool changed = true;
    while(changed){
        changed=false;
        for(unsigned i=0; i<wordCount.size()-1; ++i){
            if(wordCount.at(i) < wordCount.at(i+1)){
                swap(wordList.at(i), wordList.at(i+1));
                swap(wordCount.at(i), wordCount.at(i+1));
                changed = true;
            }
        }
    }

    cout << endl << "TOTAL WORD COUNT:\t\t\t" << totalWords;
    for(unsigned i=0; i< wordList.size(); ++i){
      if(wordCount.at(i) > 2){
            outFile << wordList.at(i) << "," << wordCount.at(i) << "\n";
            cout << wordList.at(i) << "\t\t\t" << wordCount.at(i) << endl;
        }
    }

    foodFile.close();
    outFile.close();

    cout << "END" << endl;
    return 0;
}

void cleanWord(string* word){
    for(string::iterator i = word->begin(); i != word->end(); i++)
    {
        if(!isalpha(word->at(i - word->begin())))
        {
            word->erase(i);
            --i;
        }
    }
    transform(word->begin(), word->end(), word->begin(), ::tolower);
}

