#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

static const string dataFileName= "./../food-enforcement.json";
static const string outFileName= "./../food-enforcement_REPEATS_REMOVED.json";

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

struct DateAndFirm
{
    DateAndFirm(string d, string f)
        :date(d), firm(f) {}

    string date;
    string firm;
};

int main()
{
    ifstream dataFile(dataFileName);
    ofstream outFile(outFileName);
    if(!dataFile.is_open() )
    {
        throw runtime_error("Error opening dataFile");
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Error opening outFile");
    }
    vector<DateAndFirm> dateAndFirms;
    vector<vector<string>> events;
    vector<string> event;
    string line;    //line of the DataFile
    string date;    //Date of the Recall
    string firm;    //Recalling firm
    string quan;    //Quantity string
    int added=0;        //Number of unique events
    int skipped=0;        //Number of repeated events

    bool started = false;   //Has the file reader reached results?
    bool repeat; // Is the event a repeat event?
    int ind; // if the data is a repeat, what index does the first event occour at?

    outFile << "{\n  \"meta\": {\n  },\n"; //meta string
    while(getline(dataFile, line))
    {
        cout << "."; //line << endl;
        if(!started)
        {
            started = (line==resultsStr); // If line == results the next lines are not meta data.
        }

        if(started)
        {
            event.push_back(line); // store the current line

            if(isRecallDate(&line))
            {
                repeat=false; // Event is assumed to be not a repeat
                date = line; // store the initial recall date data
            }
            else if(isFirm(&line))
            {
                firm = line; // store the firm name data
                for(int i=dateAndFirms.size()-1; i>=0; --i) // check the stored data to see if the current event is a repeat event
                {
                    DateAndFirm* df = &dateAndFirms.at(i);
                    if( (df->date == date) && (df->firm==firm) ) // the current event is a repeat
                    {
                        repeat = true;
                        ind = i;
                        break;
                    }
                }
            }
            else if(isProductQuan(&line))
            {
                quan = line.substr(productQuanStr.length(), line.length()-productQuanStr.length()-1); // store the quantity string, so it can be concatenated onto the correct event, if necessary
                //cout << quan << endl;
            }
            else if(isEnd(&line))
            {
                if(!repeat) // if the event is not a repeat event, add it to the list of events
                {
                    DateAndFirm df(date, firm);
                    dateAndFirms.push_back(df);
                    events.push_back(event);
                    ++added; // Increment the number of unique events
                }
                else // if the event is a repeat add the product quantity to the correct line
                {
                    vector<string> * eventPtr = &(events.at(ind));
                    for(unsigned i=0; i<eventPtr->size(); ++i)
                    {
                        if(isProductQuan(&(eventPtr->at(i))))
                        {
                            string* quanPtr = (&eventPtr->at(i));
                            if(quanPtr->find(quan) == string::npos) // if the quantity from the current repeated event is not in the non-repeated event
                            {
                                quanPtr->pop_back();    // remove the '"';
                                (*quanPtr) +="   ";      // add spacing
                                (*quanPtr) +=quan;      // add the quantity from the repeated event
                                (*quanPtr) +="\"";      // add the '"' back
                            }
                            break;
                        }
                    }
                    cout <<endl <<"*skipping entry*" <<endl;
                    ++skipped; // Increment the number of repeat events
                }
                cout <<endl;
                event.clear();
            }
        }
    }
    dataFile.close(); // close the dataFile

    // output all the data to the outFile
    for(unsigned i=0; i<events.size(); ++i)
    {
        vector<string>* eventPtr = &(events.at(i));
        for(unsigned j=0; j<eventPtr->size(); ++j)
        {
            outFile << eventPtr->at(j) <<"\n";
        }
    }
    outFile << "  ]\n}"; // add the correct ending to the .json file
    outFile.close(); //close the outFile

    //print out how many events were repeats
    cout << endl;
    cout << "************************************************************" <<endl;
    cout << "* Total of " << (added + skipped) << " events." << endl;
    cout << "* " << added <<" records were unique and thus added to the out file" << endl;
    cout << "* " << skipped <<" records were repeats," << endl;
    cout << "* (Repeated records were not added)" << endl;
    cout << "************************************************************" <<endl;
    cout << "END" << endl;
    return 0;
}

bool compareVariable(string* line, const string varName)
{
    return (line->substr(0, varName.length())==varName);
}

bool isRecallDate(string* line)
{
    return compareVariable(line, recallInitDate);
}
bool isFirm(string* line)
{
    return compareVariable(line, firmStr);
}

bool isProductQuan(string* line)
{
    return compareVariable(line, productQuanStr);
}

bool isStart(string* line)
{
    return compareVariable(line, startStr);
}

bool isEnd(string* line)
{
    return (compareVariable(line, endStr1) || compareVariable(line, endStr2));
}
