#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string outFileName= "./../food-enforcement_ProcessedDataTEST.json";

static const string startStr= "    {";
static const string endStr1= "    },";
static const string endStr2= "    }";
static const string firmStr= "      \"recalling_firm\": \"";
static const string recallInitDate= "      \"recall_initiation_date\": \"";
static const string productQuanStr= "      \"product_quantity\": \"";

static const string resultsStr = "  \"results\": [";

bool isStart(string* line);
bool isFirm(string* line);
bool isRecallDate(string* line);
bool isProductQuan(string* line);
bool isEnd(string* line);


int main()
{
    ifstream dataFile(dataFileName);
    ofstream outFile(outFileName);

    if(!dataFile.is_open() ){
        throw runtime_error("Error opening dataFile");
    }else if(!outFile.is_open()){
        throw runtime_error("Error opening outFile");
    }

    vector<string> lines;
    vector<string> lineBuff;
    string line;
    string prevDate;
    string prevFirm;
    string date;
    string firm;
    bool started = false;

    outFile << "{\n  \"meta\": {\n  },\n";
    int c=0;
    while(getline(dataFile, line)){
      //  cout << line <<endl;
        if(!started){
            started = (line==resultsStr);
        }
        if(started){
            if(isFirm(&line)){
                firm = line;
            }else if(isRecallDate(&line)){
                date = line;
            }

            lines.push_back(line);

            if(isEnd(&line)){
                bool firmB=(firm==prevFirm);
                bool dateB=(date==prevDate);

                if(!(dateB && firmB)){
                    for(unsigned i=0; i<lineBuff.size(); ++i){
                        outFile << lineBuff.at(i) <<"\n";
                    }
                    lineBuff = lines;
                    prevDate = date;
                    prevFirm = firm;
                }else{
                    string* ptr1;
                    for(unsigned i =0; i<lineBuff.size(); ++i){
                        if(isProductQuan(&(lineBuff.at(i)))){
                            ptr1 = &(lineBuff.at(i));
                            break;
                        }
                    }
                    for(unsigned i =0; i<lines.size(); ++i){
                        if(isProductQuan(&(lines.at(i)))){
                            (*ptr1) = (*ptr1).substr(0,(*ptr1).length()-1);
                            (*ptr1) += " ";
                            (*ptr1) += (lines.at(i).substr(productQuanStr.length(), lines.at(i).length() -productQuanStr.length()+1));
                            cout << *ptr1 <<endl;
                            break;
                        }
                    }
                    cout <<endl <<"skipping entry" <<endl;
                    ++c;
                }
                lines.clear();
            }
        }
    }
    for(unsigned i=0; i<lineBuff.size(); ++i){
        outFile << lineBuff.at(i) <<"\n";
    }

    outFile << "  ]\n}";
    dataFile.close();
    outFile.close();

    cout << endl;
    cout << "***************************" <<endl;
    cout << c <<" records removed" << endl;
    cout << "***************************" <<endl;
    cout << "END" << endl;
    return 0;
}

bool compareVariable(string* line, const string varName){
    return (line->substr(0, varName.length())==varName);
}

bool isRecallDate(string* line){
    return compareVariable(line, recallInitDate);
}
bool isFirm(string* line){
    return compareVariable(line, firmStr);
}

bool isProductQuan(string* line){
    return compareVariable(line, productQuanStr);
}

bool isStart(string* line){
    return compareVariable(line, startStr);
}

bool isEnd(string* line){
    return (compareVariable(line, endStr1) || compareVariable(line, endStr2));
}
