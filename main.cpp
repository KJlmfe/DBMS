
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <memory.h>
using namespace std;
#define namesize 20

enum datatype
{
    INT, CHAR,DOUBLE
};



class Attribute
{
public:
    string name;
    datatype type;
    int size;

    Attribute(string name, datatype type)
    {
        this->name = name;
        this->type = type;
    }

    Attribute(string name, datatype type, int size)
    {
        this->name = name;
        this->type = type;
        this->size = size;
    }
};

class table{
public:
    string name;
    vector<Attribute> Attributes;
    table()
    {
        name = "test";
        Attributes.push_back(Attribute("name",CHAR,4));
        Attributes.push_back(Attribute("phone",CHAR,5));
        char temp[100] = {"namephoned"};
        vector<string> name;
        name.push_back("name");
//        name.push_back("phone");
        insert(temp,name);
    }
    int getsize()
    {
        int size = 0;
        for (int i = 0;i < Attributes.size();i++)
        {
             size += Attributes[i].size;
        }
        return size;
    }
    void insert(char * data,vector<string> AttributesName)
    {
        char * result = new char(getsize());
        memset(result,0,getsize());
        int a = getsize();
        vector<int> datalocation;
        int temp = 0;
        for (int i = 0;i < AttributesName.size();i++)
        {
            datalocation.push_back(temp);
            for (int j = 0;j < Attributes.size();j++)
                if (Attributes[j].name == AttributesName[i])
                {
                    temp += Attributes[j].size;
                    break;
                }
            
        }
   
        int location = 0;
      
        for (int i = 0;i < Attributes.size();i++)
        {
            switch (Attributes[i].type)
            {
            case INT:
//                b = 0;
//                memcpy(result+location,&b,sizeof(int));
                break;
            case CHAR:
                result[location] = '\0';
                break;
            case DOUBLE:
//                double a = 0;
//                memcpy(result+location,&a,sizeof(double));
                break;    
            default:
                break;
            }
            for (int j = 0;j < AttributesName.size();j++)
            {
                if (AttributesName[j] == Attributes[i].name)
                {
                    memcpy(result+location,data+datalocation[j],Attributes[i].size);
                }
            }
            location += Attributes[i].size;
        }
        fstream file;
        file.open(name.c_str(),ios::out | ios::app |ios::binary);
        file.write(result,getsize());
        file.close();
        
    }
    
};
void writeTable(string tablename, int key, vector<Attribute> Table)
{
    fstream file;
    file.open("model.dat", ios::out | ios::app | ios::binary);

    file.write(tablename.c_str(), namesize); //写入表名
    file.write((char *) &key, 4); //写入主键数
    int number = Table.size();
    file.write((char *) &number, 4); //写入属性数
    for (vector<Attribute>::iterator p = Table.begin(); p != Table.end(); p++)
    {
        file.write(p->name.c_str(), namesize);
        file.write((char *) &(p->type), sizeof (datatype));      
        if (p->type == CHAR)
            file.write((char *) &(p->size), 4);
    }
    file.close();
}

void readTable()
{
    fstream file;
    file.open("model.dat", ios::in | ios::binary);
 
    char name[namesize];
    int keynumber;
    int number;

    while (file.peek() != EOF)
    {
        file.read(name, namesize); //读取表名
        cout << name << endl;
        file.read((char *) &keynumber, 4);
        cout << keynumber << endl;
        file.read((char *) &number, 4);
        cout << number << endl;
        for (int i = 0; i < number; i++)
        {
            char attrlbutename[namesize];
            datatype type;
            int size;
            file.read(attrlbutename, namesize);
            cout << attrlbutename << endl;
            file.read((char *) &type, sizeof (datatype));
            cout << type << endl;
            if (type == CHAR)
            {
                file.read((char *) &size, 4);
                cout << size << endl;
            }
        }
        cout << endl;
    }
    file.close();





}

int main(int argc, char** argv)
{

//    vector<Attribute> temp;
//    temp.push_back(Attribute("name", CHAR, 3));
//    temp.push_back(Attribute("phone", INT));
//    writeTable("person", 1, temp);
//    readTable();
    table a;
    
    
    return 0;
}
