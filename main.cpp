#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <memory.h>
#include <cstdlib>

using namespace std;

#define NAMESIZE 20
#define INTSIZE  4

#include "Attribute.h"
#include "Table.h"
#include "DataBase.h"


int main(int argc, char** argv)
{

    //测试model
    DataBase data;

    vector<Attribute> temp;
    temp.push_back(Attribute("name", CHAR, 3));
    temp.push_back(Attribute("phone", INT));
    data.create_table("person", 1, temp);

    data.read_table();

    data.show_database();


    return 0;
}

