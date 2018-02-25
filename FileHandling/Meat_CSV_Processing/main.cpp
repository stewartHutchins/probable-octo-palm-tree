#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string dataFileName = "./FSIS Recall.csv";
string outFileName = "./FSIS Recall_standardized.csv";

static int total_unknown =0;
static const string meat_types[] = {"pork","chicken","turkey","veal", "beef", "fish"};
//Salami and Soppressata,ostrich fish, ham

//function declaration
vector<string> splitByComma(string line);
int contains(string str, string arg);
vector<bool> findType(string str);



int main()
{
    ifstream inFile(dataFileName);
    ofstream outFile(outFileName);
    if(!inFile.is_open()){
        throw runtime_error("Couldn't open inFile: " + dataFileName);
    }else if(!outFile.is_open()){
        throw runtime_error("Couldn't open outFile: " + outFileName);
    }
    string line;
    unsigned lineCount = 0;
    while(getline(inFile, line)){
        if(lineCount ==0){
            outFile << line << ",Type 1,Type 2,Preparation,";
            for(unsigned i=0; i<sizeof(meat_types)/sizeof(meat_types[i]); ++i)
            {
                outFile << meat_types[i] << ",";
            }
            outFile << "\n";
        }else{
            outFile << line << ",";
            vector<string> section = splitByComma(line);
            vector<bool> types = findType(section.at(6));
            for(unsigned i =0; i<types.size(); ++i){
                outFile << types.at(i) << ",";
            }
            outFile << "\n";
        }
        ++lineCount;
    }
    outFile.close();

    return 0;
}



vector<bool> findType(string str){
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    vector<bool> vec;
    int c=0;
    for(unsigned i=0; i<sizeof(meat_types)/sizeof(meat_types[0]); ++i)
    {
        int b = contains(str, meat_types[i]);
        if(b){
            cout << meat_types[i]  <<endl;
        }
        vec.push_back(b);
        c+=b;
    }
    if(c==0)
    {
        if(contains(str, "sausage") || contains(str, "ham"))
        {
            vec.at(0) = 1;
            ++c;
        }
    }
    if(c==0){
        ++total_unknown;
    }
    return vec;
}

int contains(string str, string arg){
    if(str.find(arg) !=string::npos)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


vector<string> splitByComma(string line){
    vector<string> vec;
    int ind;
    while((ind = line.find(",")) != string::npos){
        ++ind;
        vec.push_back(line.substr(0, ind));
        line = line.substr(ind, line.length()-ind);
    }
    vec.push_back(line);
    return vec;
}
