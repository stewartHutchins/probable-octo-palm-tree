#ifndef BUISNESSDATE_H
#define BUISNESSDATE_H

#include <string>

class BuisnessDate
{
    public:
        BuisnessDate(std::string, std::string);
        std::string getDate();
        std::string getRecallingFirm();
    protected:

    private:
        std::string date;
        std::string recallingFirm;

};

#endif // BUISNESSDATE_H
