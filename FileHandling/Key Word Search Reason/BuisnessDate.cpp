#include "BuisnessDate.h"
#include <string>

using namespace std;

BuisnessDate::BuisnessDate(string rf, string d)
: recallingFirm(rf), date(d)
{
}

string BuisnessDate::getDate()
{
    return date;
}

string BuisnessDate::getRecallingFirm()
{
    return recallingFirm;
}
