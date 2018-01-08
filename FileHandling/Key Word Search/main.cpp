#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string outFileName= "./keyWords.txt";
static const string commonFileName= "./commonWords.txt";


static const string productDesc= "\"product_description\":";
static const string codeInfo= "codeinfo";

//function declaration
void cleanWord(string* word);

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

    unsigned totalWords = 0;
    while(foodFile >> word){
        if(word.substr(0,productDesc.length())==productDesc ){
            vector<string> tempWordList;
            while(word != codeInfo){
                ++totalWords;
                foodFile >> word;
                cleanWord(&word);
                bool common = false;
                for(unsigned i = 0; i< commonList.size(); ++i){
                    if(commonList.at(i) ==word){
                        common = true;
                        break;
                    }
                }
                if(!common){
                    bool added = false;
                    for(unsigned i=0; i<tempWordList.size(); ++i){
                        if(word == tempWordList.at(i)){
                            added = true;
                            break;
                        }
                    }
                    if(!added){
                        tempWordList.push_back(word);
                        cout << ". ";
                    }
                }

            }
            for(unsigned i=0; i<tempWordList.size(); ++i){
                bool added = false;
                for(unsigned j=0; j<wordList.size(); ++j){
                    if(tempWordList.at(i) == wordList.at(j)){
                        ++wordCount.at(j);
                        added=true;
                        break;
                    }
                }
                if(!added){
                    wordList.push_back(tempWordList.at(i));
                    wordCount.push_back(1);
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
            outFile << wordList.at(i) << "\t\t\t" << wordCount.at(i) << "\n";
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
