#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib>
#include "Table.h"
#include "DataBase.h"
using namespace std;

int main(int argc, char** argv)
{

    //测试model
    DataBase data;

    vector<Attribute> temp;
    temp.push_back(Attribute("name", CHAR, 3));
    temp.push_back(Attribute("phone", INT,4));
    data.create_table("person", 1, temp);
    data.read_table();
    data.show_database();

    vector<string> attri;
    vector<string> value;
    attri.push_back("name");
    attri.push_back("phone");
    value.push_back("Tom");
    value.push_back("1353");
    data.tables[0].Insert(attri, value);
    data.tables[0].Delete("name","JKKK");
    data.tables[0].update("phone","1353","name","JKKK");
    return 0;
}

