#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib>
#include "Table.h"
#include "DataBase.h"
DataBase data;
#include "Sql.h"
using namespace std;

int main(int argc, char** argv)
{

    //    sql_input();

    DataBase data;
    vector<Attribute> temp1;
    temp1.push_back(Attribute("name", CHAR, 4));
    temp1.push_back(Attribute("phone", INT, 4));
    data.create_table("person", 1, temp1);

    vector<Attribute> temp2;
    temp2.push_back(Attribute("name", CHAR, 4));
    temp2.push_back(Attribute("id", CHAR, 4));
    data.create_table("student", 1, temp2);
    //    data.read_table();
    data.show_database();

    vector<string> attri1;
    vector<string> value1;
    attri1.push_back("name");
    attri1.push_back("phone");
    value1.push_back("Toms");
    value1.push_back("1111");

    vector<string> attri2;
    vector<string> value2;
    attri2.push_back("name");
    attri2.push_back("phone");
    value2.push_back("Jack");
    value2.push_back("2222");

    data.tables[0].Insert(attri1, value1);
    data.tables[0].Insert(attri2, value2);
    data.tables[0].show_table();
    vector<int> temps;
    temps.push_back(1);
    data.Projection(data.tables[0].attributes,temps).show_table();
//    data.tables[0].show_table();



    //    data.tables[1].Insert(attri1, value1);
    //    data.tables[1].Insert(attri2, value2);

    //    vector<int> query;
    //    query.push_back(1);
    //    query.push_back(0);
    //    query.push_back(2);
    //
    //    data.tables[0].show_table();
    //    data.tables[1].show_table();
    //
    //    data.Equi_Join(data.tables[0], data.tables[1], query, query, Attribute("name", CHAR, 4), Attribute("name", CHAR, 4)).show_table();
    //    data.tables[0].name = "temp";
    //
    //    temp2.push_back(Attribute("phone", CHAR, 4));
    //    data.tables[0].attributes = temp2;
    //    data.temp_table.show_table();
    //
    //    data.tables[0].Delete("name", "JKKK");
    //    data.tables[0].update("phone", "1353", "name", "JKKK");
    return 0;
}

