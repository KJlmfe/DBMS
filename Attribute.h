
#include "define.h"
#include <string>
#include <iostream>
using namespace std;
class Attribute
{
public:
    string name;       
    DataType type;
    int size;
    Attribute()
    {
        
    }


    Attribute(string name,DataType type,int size)
    {
        this->name = name;
        this->type = type;
        this->size = size;
    }

};


