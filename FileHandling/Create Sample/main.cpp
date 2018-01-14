#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string outFileName= "./200sample.txt";

static const string recallReasonStr="      \"reason_for_recall\": \"";
static const string productDescStr="      \"product_description\": \"";

static double numberOfSamplesWanted = 200.0;
static double proportion = numberOfSamplesWanted/14566.0;

void toLowerCase(string* str );

int main()
{
    ifstream dataFile(dataFileName);
    ofstream outFile(outFileName);

    if(!dataFile.is_open() ){
        throw runtime_error("Error opening dataFile");
    }else if(!outFile.is_open()){
        throw runtime_error("Error opening outFile");
    }

    double random;
    string line;
    while(getline(dataFile, line)){
        random = ((double)rand() / (RAND_MAX));
        if(random <= proportion){
            if(line.substr(0,recallReasonStr.length())==recallReasonStr){
                toLowerCase(&line);
                outFile << "\n" << line <<"\n" <<"RECALL_REASON_SHORT: " <<"\n";
                cout <<line <<endl;
            }else if(line.substr(0,productDescStr.length())==productDescStr){
                toLowerCase(&line);
                outFile <<"\n"<< line <<"\n" <<"PRODUCT_SHORT: " <<"\n";
                cout <<line <<endl;
            }
        }
    }

    dataFile.close();
    outFile.close();

    cout << "END" << endl;
    return 0;
}

void toLowerCase(string* str ){
    transform(str->begin(), str->end(), str->begin(), ::tolower);
}
