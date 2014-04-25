#include <iostream>
#include <sstream>

#include "PrestaApi.h"


using namespace std;


int main()
{
    PrestaApi prestaApi;
    prestaApi.initApi("RB9WCBTSVZCZOASIJ1LUBAVII1PS8IGE", "", "http://www.presta.mikran.pl/prestashop/api");

    vector<string> categories;
    //categories.push_back("Kategoria_1");
    //categories.push_back("Kategoria_2");

    prestaApi.addCategory(categories);


    return 0;
}

