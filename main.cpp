
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <memory.h>
#include <cstdlib>
using namespace std;
#define namesize 20

//枚举数据类型

enum DataType
{
    INT, CHAR, DOUBLE
};


//属性

class Attribute
{
public:
    string name;
    DataType type;
    int size;

    Attribute(string name, DataType type)
    {
        this->name = name;
        this->type = type;
    }

    Attribute(string name, DataType type, int size)
    {
        this->name = name;
        this->type = type;
        this->size = size;
    }
};

//表

class Table
{
public:
    string name; //表名
    int key_num;
    vector<Attribute> attributes; //属性数组

    Table(string name, int keynumber, vector<Attribute> attributes)
    {
        this->name = name;
        this->key_num = keynumber;
        this->attributes = attributes;
    }

    Table()
    {
        //测试数据
        //        name = "test";
        //        attributes.push_back(Attribute("name", CHAR, 4));
        //        attributes.push_back(Attribute("phone", CHAR, 5));
        //        vector<string> attri_name;
        //        vector<string> value;
        //        attri_name.push_back("name");
        //        value.push_back("Zhang");
        //        Insert(attri_name, value);
    }

 
    //显示当前表结构
    void describe()
    {
        cout << "*******************************" << endl;
        cout << name << endl;
        for (int i = 0; i < attributes.size(); i++)
        {
            cout << attributes[i].name << " ";
            if (attributes[i].type == CHAR)
            {
                cout << "char(" << attributes[i].size << ")";
            }
            if (attributes[i].type == INT)
            {
                cout << "int";
            }
            cout << endl;
        }
        cout << "*******************************" << endl;
    }

    int getsize()
    {
        int size = 1;
        for (int i = 0; i < attributes.size(); i++)
        {
            if (attributes[i].type == CHAR)
                size += 1;
            size += attributes[i].size;
        }
        return size;
    }

    //将从sql语句中读取的数据，转换成二进制数据，存放到string中
    //attri为属性，value为数据

    string binary(Attribute attri, string value)
    {
        string output;
        if (attri.type == CHAR)
        {
            output = value;
            output.resize(attri.size + 1, '\0');
            return output;
        }

        if (attri.type == INT)
        {
            char * temp = new char(4);
            int num = atoi(value.c_str());
            memcpy(temp, &num, 4);
            output.append(temp, 4);
            delete temp;
            return output;
        }
    }

    //写入对应属性的二进制空数据

    string binary_empty(Attribute attri)
    {
        string output;
        if (attri.type == CHAR)
        {
            output.resize(attri.size, '\0');
            return output;
        }

        if (attri.type == INT)
        {
            char * temp = new char(4);
            int num = 0;
            memcpy(temp, &num, 4);
            output.append(temp, 4);
            delete temp;
            return output;
        }

    }
    //插入一个元组tuple，AttributesName，是属性名数组，属性名顺序是任意的，data中，是属性对应的要插入的数据
    //insert into tablename 

    void Insert(vector<string> attri_name, vector<string> value)
    {
        string result;//存储最终二进制数据
        //写入删除位，'0'为已经删除，'1'为存在
        result += '1';
        for (int i = 0; i < attributes.size(); i++)
        {
            int j;
            //判断当前属性，是否为要插入的属性
            for (j = 0; j < attri_name.size(); j++)
            {
                if (attri_name[j] == attributes[i].name)
                {
                    //插入数据
                    result += binary(attributes[i], value[j]);
                }
            }
            //当前属性不是要插入的数据，则插入空数据
            if (j == attri_name.size())
                result += binary_empty(attributes[i]);
        }

        fstream file;
        file.open(name.c_str(), ios::out | ios::app | ios::binary);
        file.write(result.c_str(), result.size());
        file.close();
    }



    //根据提供的属性名和属性值，进行查找，并删除对应列,属性值的类型，要结合attributes,自行判断
    //对应delete语句：delete from tablename where attri_name = value

    bool Delete(string attri_name, string value)
    {

    }

    //更新操作，对应update语句：
    //update tablename set attri_name2 = value2 where attri_name1 = value1

    void update(string attri_name1, string value1, string attri_name2, string value2)
    {

    }

};

//将表结构，写入model中



//从model中，读取表结构

class DataBase
{
public:
    vector<Table> tables;

    DataBase()
    {
        read_table();
    }

    //显示数据库中的所有表

    void show_database()
    {
        for (int i = 0; i < tables.size(); i++)
        {
            tables[i].describe();
        }
    }

    //从文件中读取表结构，初始化数据库

    void read_table()
    {
        tables.clear();
        fstream file;
        file.open("model.dat", ios::in | ios::binary);

        char name[namesize];
        int key_num;
        int number;
        DataType type;
        int type_size;

        char attri_name[namesize];

        while (file.peek() != EOF)
        {
            Table table;
            file.read(name, namesize); //读取表名
            table.name.append(name);

            file.read((char *) &key_num, 4);
            table.key_num = key_num;

            file.read((char *) &number, 4);

            for (int i = 0; i < number; i++)
            {
                file.read(attri_name, namesize);
                file.read((char *) &type, sizeof (DataType));
                if (type == CHAR)
                {
                    file.read((char *) &type_size, 4);
                }
                else
                {
                    type_size = 4;
                }
                table.attributes.push_back(Attribute(string(attri_name), type, type_size));
            }
            tables.push_back(table);

        }
        file.close();
    }


    //建立新表

    void create_table(string table_name, int key_num, vector<Attribute> attributes)
    {
        //将新表写入内存
        Table table;
        table.name = table_name;
        table.key_num = key_num;
        table.attributes = attributes;
        tables.push_back(table);

        //将新表写入文件
        fstream file;
        file.open("model.dat", ios::out | ios::app | ios::binary);

        file.write(table_name.c_str(), namesize); //写入表名
        file.write((char *) &key_num, 4); //写入主键数
        int number = attributes.size();
        file.write((char *) &number, 4); //写入属性数量
        for (vector<Attribute>::iterator p = attributes.begin(); p != attributes.end(); p++)
        {
            file.write(p->name.c_str(), namesize);
            file.write((char *) &(p->type), sizeof (DataType));
            if (p->type == CHAR)
                file.write((char *) &(p->size), 4);
        }
        file.close();
    }

};

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

