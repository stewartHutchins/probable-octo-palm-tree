#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

static const string fileName= "./../food-enforcement.json";
static const string productQuanitity= "      \"product_quantity\": \"";
static const string productDesc= "      \"product_description\": \"";



int main()
{
    ifstream foodFile(fileName);
    ofstream outFile("foodQuantityTypes.txt");
    if(!foodFile.is_open()){
        throw runtime_error("Error opening foodFile");
    }else if(!outFile.is_open()){
        throw runtime_error("Error opening outFile");
    }

    string line;
    while(getline(foodFile, line)){
        //get quanitiy
        if(line.substr(0,productQuanitity.length())==productQuanitity ){
            cout << line.substr(productQuanitity.length(), line.length()-productQuanitity.length()-1) << "\n";
        }//get desc
        else if(line.substr(0,productDesc.length())==productDesc ){
          //  cout << line.substr(productDesc.length(), line.length()-productDesc.length()-1) << "\n";
        }
    }

    foodFile.close();
    outFile.close();

    cout << "END" << endl;
    return 0;
}
