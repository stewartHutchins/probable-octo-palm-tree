#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

string dataFileName = "./FSIS Recall.csv";
string outFileName = "./FSIS Recall_standardized.csv";

static int total_unknown =0;
static const string meat_types[] = {"pork","poultry","lamb","beef","fish"};
//Salami and Soppressata,ostrich fish, ham

//function declaration
vector<string> splitByComma(string line);
int contains(string str, string arg);
vector<bool> findType(string str);


struct Event
{
    Event(string r_n, string o_d, string yr,string c_R,string c_A, int p_r,string p, string p_t)
        : recall_num(r_n), open_date(o_d), year(yr), class_r(c_R),class_a(c_A),pounds_recalled(p_r),product(p),problem_type(p_t)
    {   }

    string recall_num, open_date, year, class_r,class_a,product,problem_type;
    int pounds_recalled;

    string toString()
    {
        stringstream ss;
        ss <<pounds_recalled;
        return recall_num+","+open_date+","+year+","+class_r+","+class_a+","+ss.str()+","+product+","+problem_type+",";
    }
};



int main()
{
    ifstream inFile(dataFileName);
    ofstream outFile(outFileName);

    if(!inFile.is_open())
    {
        throw runtime_error("Couldn't open inFile: " + dataFileName);
    }
    else if(!outFile.is_open())
    {
        throw runtime_error("Couldn't open outFile: " + outFileName);
    }

    vector<Event> events;
    string line;
    unsigned lineCount = 0;
    while(getline(inFile, line))
    {
        if(lineCount ==0)
        {
            outFile << line;
            for(unsigned i=0; i<sizeof(meat_types)/sizeof(meat_types[i]); ++i)
            {
                outFile << meat_types[i] << ",";
            }
            outFile << "\n";
        }
        else
        {
            vector<string> section = splitByComma(line);
            for(int i=0; i<section.size(); ++i)
            {
                cout << i << ")\t" << section.at(i) << endl;
            }
            cout <<endl;
            stringstream ss(section.at(5));
            int add;
            ss >> add;
            bool flag = false;
            for(unsigned i =0; i< events.size(); ++i)
            {
                if(events.at(i).open_date == section.at(1) && events.at(i).product == section.at(6) && events.at(i).problem_type == section.at(7))
                {
                    if(!ss.fail())
                    {
                        events.at(i).pounds_recalled += add;
                        flag = true;
                        break;
                    }
                }
            }
            if(!flag)
            {
                Event e(section.at(0), section.at(1),section.at(2),section.at(3),section.at(4),add,section.at(6),section.at(7));
                events.push_back(e);
            }
        }
        ++lineCount;
    }

    cout << endl << "FINISHED READING FILE" << endl;

    for(unsigned i=0; i<events.size(); ++i)
    {
        cout <<"Reading " << i << " th element" <<endl;
        Event* eventPtr = &(events.at(i));
        outFile << eventPtr->toString();
        vector<bool> types = findType(eventPtr->product);

        for(unsigned j=0; j<types.size(); ++j)
        {
            outFile << types.at(j) << ",";
        }
        outFile <<endl;
    }

    outFile.close();
    cout << endl << "Total Unknown meats: " << total_unknown << endl;

    return 0;
}



vector<bool> findType(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    vector<bool> vec;
    int c=0;
    for(unsigned i=0; i<sizeof(meat_types)/sizeof(meat_types[0]); ++i)
    {
        int b = contains(str, meat_types[i]);
        vec.push_back(b);
        c+=b;

    }
    if(c==0)
    {
        if(contains(str, "sausage") || contains(str, "frank") || contains(str, "hot dog"))
        {
            vec.at(0) = 1;
            vec.at(1) = 1;
            ++c;
        }
        else if(contains(str, "ham") ||contains(str, "bacon") )
        {
            vec.at(0) = 1;
            ++c;
        }
        else if(contains(str, "turkey") || contains(str, "chicken") || contains(str, "duck") )
        {
            vec.at(1) = 1;
            ++c;
        }
        else if(contains(str, "steak") || contains(str, "veal"))
        {
            vec.at(3) = 1;
            ++c;
        }
        else if(contains(str, "pepperoni") ||contains(str, "salami"))
        {
            vec.at(0) = 1;
            vec.at(4) = 1;
            ++c;
        }
    }
    if(c==0)
    {
        ++total_unknown;
    }
    return vec;
}

int contains(string str, string arg)
{
    if(str.find(arg) != string::npos)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


vector<string> splitByComma(string line)
{
    vector<string> vec;
    int ind;

    while((ind = line.find(",")) != string::npos)
    {
        ++ind;
        //cout << line <<endl;
        vec.push_back(line.substr(0, ind-1));
        line = line.substr(ind, line.length()-ind);
    }
    vec.push_back(line);
    return vec;
}
