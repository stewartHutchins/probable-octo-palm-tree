#include <iostream>
#include <string>
#include <vector>
#include <ReClaM/FFNet.h>
#include <ReClaM/createConnectionMatrix.h>

using namespace std;
const string labeledDataString = "./../200Sample.txt";

const string descStr = "      \"product_description\": \"";
const string shortDescStr = "PRODUCT_SHORT: ";
const string recallStr = "      \"reason_for_recall\": \"";
const string shortRecallStr = "RECALL_REASON_SHORT: ";


//function declaration
bool isVar(string* line, string* var);
void cleanString(string *line);
void addWords(string line, vector<string>* wordVector);

int main(){

	vector<string> wordsDesc;
	vector<string> wordsReason;
	vector<string> productDescOutputs;
	vector<string> reasonDescOutputs;

	ifstream dataFile(labeledDataString);
		
	string line;
	
	while(getline(dataFile, line)){
		
		if(isVar(&line, &descStr)){
			addWords(line, &wordsDesc);
		}else if(isVar(&line, &shortDescStr)){
			line = line.substr(shortDescStr.size(), line.size()-shortDescStr.size());
			bool added = false;
			for(unsigned i=0; i<productDescOutputs.size(); ++i ){
				if(productDescOutputs.at(i) == line){
					productDescOutputs.push_back();
				}
			}
		}else if(isVar(&line, &recallStr)){
			addWords(line, &wordsReason);
		}else if(isVar(&line, &shortDescStr)){
			line.substr(shortDescStr.size(), line.size()-shortDescStr.size());
		}
	}

	return EXIT_SUCCESS;
}


void addWords(string line, vector<string>* wordVector){
    line = line.substr(descStr.length(), line.length()-descStr.length());
	cleanString(&line);
	vector<string> wordsInStr = splitIntoWords(line);
	for(unsigned i=0; i<wordsInStr.size(); ++i){
		bool added = false;
		for(unsigned j=0; j<wordVector->size(); ++j){
			if(wordsInStr.at(i) == wordVector->at(j)){
				added = true;
				break;
			}
		}
		if(!added){
			wordVector->push_back(wordsInStr.at(i));
		}
	}
}


vector<string> splitIntoWords(string str){
    string buf; // Have a buffer string
    stringstream ss(str); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf){
        tokens.push_back(buf);
	}
	return tokens;
}


bool isVar(string* line, const string* var){
	if(line->substr(0,var->length) == *var){
		return true;
	}else{
		return false;
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

