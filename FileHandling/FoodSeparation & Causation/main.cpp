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

static const string recallReasonStr="      \"reason_for_recall\": \"";
static const string productDescStr="      \"product_description\": \"";

static const string reasons[] ={"Undeclared Allergens","Mould/Spoiled","Salmonella","Machine Fragments","Other Contamination","Unknown"};


static const string alergens[] = {"almond","walnut","pecan","cashew","pistachio","peanut","nut","soy","egg","wheat","dye","fish","shellfish", "milk","dairy"};

//function declaration
void cleanWord(string* word);

struct RecallInfo{

    RecallInfo(string p){
        product=p;
        for(int i=0; i<sizeof(reasons)/sizeof(reasons[0]);++i){
            causes.push_back(reasons[i]);
            causeCount.push_back(0);
        }
        subCause.push_back(reasons[5]);
        subCount.push_back(0);
    }
    string product;
    vector<string> causes;
    vector<string> subCause;
    vector<int> causeCount;
    vector<int> subCount;
};

int main()
{
    ifstream dataFile(dataFileName);
    ifstream typeFile(foodTypesFileName);

    if(!dataFile.is_open() ){
        throw runtime_error("Error opening dataFile");
    }else if(!typeFile.is_open()){
        throw runtime_error("Error opening typeFile");
    }

    vector<vector<RecallInfo>> foodTypes;
    string line;
    while(getline(typeFile, line)){
        if(line.substr(0,2) =="//"){
            //do nothing
        }else{
            vector<RecallInfo> vec;
            int ind =0;
            for(int i=0; i<line.length(); ++i){
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
    vector<RecallInfo> vec;
    vec.push_back(reasons[5]);
    foodTypes.push_back(vec);

    for(unsigned i =0; i<foodTypes.size(); ++i){
        for(unsigned j=0; j<foodTypes.at(i).size(); ++j){
            cout << foodTypes.at(i).at(j).product <<endl;
        }
    }
    cout << endl << endl;

    string reason;
    string subreason;

    while(getline(dataFile, line)){

        if(line.substr(0,recallReasonStr.length())==recallReasonStr){
            cleanWord(&line);
            if(line.find("undeclared") !=string::npos ||line.find("allergen") !=string::npos){
                reason = reasons[0];
                int ind[]={line.find("undeclared"), line.find("allergen"), line.find("undeclared") +10, line.find("allergen")+8};
                int mi=line.length();
                int ma=0;
                for(unsigned i=0; i<(sizeof(ind)/sizeof(ind[0])); ++i ){
                    if(ind[i] !=string::npos && ind[i]<mi){
                        mi = ind[i];
                    }else if(ind[i] !=string::npos && ind[i]>ma){
                        ma = ind[i];
                    }
                }
                string sub;
                subreason = "";
                bool found=false;
                while(!found){
                    sub = line.substr(mi, ma-mi);
                    mi-=5;
                    ma+=5;
                    if(mi<0){
                        mi=0;
                    }
                    if(ma>line.length()-1){
                        ma=line.length()-1;
                    }
                    for(int i=0; i<(sizeof(alergens)/sizeof(alergens[0])); ++i){
                        if(sub.find(alergens[i]) !=string::npos){
                            subreason = alergens[i];
                            found =true;
                        }
                    }
                    if(mi==0 && ma==(line.length()-1) && subreason.empty()){
                        subreason = reasons[5];
                        found=true;
                    }
                }
            }else if(line.find("mould") !=string::npos || line.find("spoil") !=string::npos){
                reason = reasons[1];
                subreason = reasons[1];
            }else if(line.find("salmonella") !=string::npos ||line.find("listeria") !=string::npos ||line.find("monocytogenes") !=string::npos ||line.find("coli") !=string::npos){
                reason = reasons[2];
                subreason = reasons[2];
            }else if(line.find("glass") !=string::npos || line.find("metal") !=string::npos || line.find("cloth") !=string::npos ||line.find("piece") !=string::npos || line.find("wire") !=string::npos ||line.find("production equipment") !=string::npos ||line.find("fragment") !=string::npos || line.find("glass chip") !=string::npos){
                reason =  reasons[3];
                if(line.find("metal") !=string::npos ||line.find("wire") !=string::npos){
                    subreason = "Metal";
                }else if( line.find("glass") !=string::npos){
                    subreason = "Glass";
                }else if( line.find("cloth") !=string::npos){
                    subreason = "Cloth";
                }else{
                    subreason = reasons[5];
                }
            }else if(line.find("contaminat") !=string::npos ){
                reason = reasons[4];
                subreason = reasons[4];
            }else{
                reason = reasons[5];
                subreason = reasons[5];
            }
        }else if(line.substr(0,productDescStr.length())==productDescStr){
            cleanWord(&line);
            bool added = false;
            //find food type
            for(int i=0; i<foodTypes.size(); ++i){
                for(int j=0; j<foodTypes.at(i).size(); ++j){
                    //ri = correct food
                    RecallInfo* ri =&foodTypes.at(i).at(j);
                    int ind=line.find(ri->product);
                    if(ind !=string::npos){
                        for(int k=0; k<ri->causes.size(); ++k){
                            cout << ".";
                            if(ri->causes.at(k) ==reason){
                                ++(ri->causeCount.at(k));
                                for(int l=0; l<ri->subCause.size(); ++l){
                                    cout << ".";
                                    if(ri->subCause.at(l) == subreason){
                                        ++(ri->subCount.at(l));
                                        added = true;
                                        break;
                                    }
                                }
                                if(!added){
                                    ri->subCause.push_back(subreason);
                                    ri->subCount.push_back(1);
                                }
                                added=true;
                                cout << endl << ri->product << "," << reason << "," << subreason<<endl;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
                                 cout << "STUCK YOYOYOYOOY";

    dataFile.close();

    ofstream outputFile(outFileName);
    ofstream outputFileSubCause(outFileNameSub);
    int overallTotal[sizeof(reasons)/sizeof(reasons[0])];
    for(int i=0; i<sizeof(overallTotal)/sizeof(overallTotal[0]); ++i){
        overallTotal[i] =0;
    }
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

