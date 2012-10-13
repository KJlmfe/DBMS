
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


//属性
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

//表
class table{
public:
    string name;//表名
    vector<Attribute> Attributes;//属性数组
    table()
    {
        //测试数据
        name = "test";
        Attributes.push_back(Attribute("name",CHAR,4));
        Attributes.push_back(Attribute("phone",CHAR,5));
        char temp[100] = {"namephoned"};
        vector<string> name;
        name.push_back("name");
//        name.push_back("phone");
        insert(temp,name);
    }
    //计算一个元组占用的空间
    int getsize()
    {
        int size = 0;
        for (int i = 0;i < Attributes.size();i++)
        {
             size += Attributes[i].size;
        }
        return size;
    }
    
    //插入一个元组，AttributesName，是属性名数组，属性名顺序是任意的，data中，是属性对应的要插入的数据
    
    void insert(char * data,vector<string> AttributesName)
    {
        int size = getsize();
        char * result = new char(size + 1);//建立空间，用来临时存放元组数据
        memset(result,0,size+1);//清空
        
        //计算AttributesName中的元组所对应的数据，在char data中的位置
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
   
        int location = 0;//数据指针
        //写入删除位
        result[0] = 1;
        location++;//移动数据指针
      
        for (int i = 0;i < Attributes.size();i++)
        {
            //数据为空的话，写入初始数据
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
            //如果当前元组，包含要插入的数据，则将要插入的数据，从data中读取，写入result中
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
        file.write(result,size + 1);
        file.close();
        
    }
    
};

//将表结构，写入model中
void writeTable(string tablename, int key, vector<Attribute> Table)
{
    fstream file;
    file.open("model.dat", ios::out | ios::app | ios::binary);

    file.write(tablename.c_str(), namesize); //写入表名
    file.write((char *) &key, 4); //写入主键数
    int number = Table.size();
    file.write((char *) &number, 4); //写入属性数量
    for (vector<Attribute>::iterator p = Table.begin(); p != Table.end(); p++)
    {
        file.write(p->name.c_str(), namesize);
        file.write((char *) &(p->type), sizeof (datatype));      
        if (p->type == CHAR)
            file.write((char *) &(p->size), 4);
    }
    file.close();
}

//从model中，读取表结构
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

