/* 
 * File:   Table.h
 * Author: xiaose
 *
 * Created on 2012年10月17日, 下午5:14
 */

#ifndef TABLE_H
#define	TABLE_H
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "define.h"
#include "Attribute.h"
#include <memory.h>
#include "Condition.h"

using namespace std;

class Table
{
public:
    string name; //表名
    int key_num; //主键个数(默认前key_num个是该表主键)
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
        //        attributes.push_back(Attribute("name", CHAR, INTSIZE));
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

    //该表一条记录的固定大小

    int get_record_size()
    {
        int size = 1;
        for (int i = 0; i < attributes.size(); i++)
        {
            size += attributes[i].size;
        }
        return size;
    }

    //将从sql语句中读取的数据，转换成二进制数据，存放到string中
    //attri为属性，value为数据

    string binary_to_string(Attribute attri, string value)
    {
        string output;
        if (attri.type == INT)
        {
            int num;
            memcpy(&num, value.c_str(), INTSIZE);
            stringstream ss;
            string str;
            ss << num;
            ss >> output;
            return output;
        }

        if (attri.type == CHAR)
            return value;

    }

    string string_to_binary(Attribute attri, string value)
    {
        string output;
        if (attri.type == CHAR)
        {
            output = value;
            output.resize(attri.size, '\0');
            return output;
        }

        if (attri.type == INT)
        {
            char * temp = new char(INTSIZE);
            int num = atoi(value.c_str());
            memcpy(temp, &num, INTSIZE);
            output.append(temp, INTSIZE);
            delete temp;
            return output;
        }
    }

    //写入对应属性的二进制空数据

    string string_to_empty_binary(Attribute attri)
    {
        string output;
        if (attri.type == CHAR)
        {
            output.resize(attri.size, '\0');
            return output;
        }

        if (attri.type == INT)
        {
            char * temp = new char(INTSIZE);
            int num = 0;
            memcpy(temp, &num, INTSIZE);
            output.append(temp, INTSIZE);
            delete temp;
            return output;
        }

    }
    //插入一个元组tuple，AttributesName，是属性名数组，属性名顺序是任意的，data中，是属性对应的要插入的数据
    //insert into tablename 

    void Insert(vector<string> attri_name, vector<string> value)
    {
        string result; //存储最终二进制数据
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
                    result += string_to_binary(attributes[i], value[j]);
                    break;
                }
            }
            //当前属性不是要插入的数据，则插入空数据
            if (j == attri_name.size())
                result += string_to_empty_binary(attributes[i]);
        }

        fstream file;
        file.open(name.c_str(), ios::out | ios::app | ios::binary);
        file.write(result.c_str(), result.size());

        file.close();
    }

    //根据提供的属性名和属性值，进行查找，并删除对应列,属性值的类型，要结合attributes,自行判断
    //对应delete语句：delete from tablename where attri_name = value

    vector<int> search(Condition condition)
    {

    }

    vector<int> search(vector<Condition> condition)
    {
        fstream file;
        int i, j;
        file.open(name.c_str(), ios::in | ios::binary);
        file.seekg(0, ios::end);
        int record_num = file.tellg() / get_record_size(); //计算元组总条数            
        vector<int> output;
        vector<int> attri_p;
        vector<Attribute> attri;
        int temp = 0;
        for (j = 0; j < condition.size(); j++)
        {
            for (i = 0; i < attributes.size(); i++)
            {
                if (attributes[i].name == condition[j].attri_name)
                    break;
                else
                    temp += attributes[i].size;
            }
            attri.push_back(attributes[i]);
            attri_p.push_back(temp);
            temp = 0;
        }

        int record_size = get_record_size();

        char is_delete;
        int x;
        int location_v = 0;
        for (i = 0; i < record_num; i++)
        {
            bool matched = true;
            file.seekg(location_v, ios::beg);
            file.read(&is_delete, 1);
            if (is_delete == '0')
                continue;
            for (j = 0; j < condition.size(); j++)
            {
                x = location_v + attri_p[j] + 1;
                file.seekg(x, ios::beg);
                char *temp = new char(attri[j].size);
                file.read(temp, attri[j].size);
                //                string data(temp, attri[j].size);
                int ope_result = strncmp(temp, condition[j].value.c_str(), attri[j].size);
                switch (condition[j].operate)
                {
                case '=':
                    if (ope_result != 0)
                        matched = false;
                    break;
                case '>':
                    if (ope_result <= 0)
                        matched = false;
                    break;
                case '<':
                    if (ope_result >= 0)
                        matched = false;
                    break;
                default:
                    break;
                }
            }
            if (matched)
                output.push_back(i);
            location_v += record_size;
        }

        return output;

    }

    bool Delete(string attri_name, string value)
    {
        vector<int> delete_queue;

        delete_queue = search(Condition(attri_name, '=', value));
        int offset;
        fstream file;
        file.open(name.c_str(), ios::out | ios::binary | ios::in);

        char temp = '0';
        for (int i = 0; i < delete_queue.size(); i++)
        {
            offset = delete_queue[i] * get_record_size();
            file.seekg(offset, ios::beg);
            //            fseek(file, offset, SEEK_SET); //将文件指针指向要删除记录的起始位

            file.write(&temp, 1); //写入删除位，'0'为已经删除，'1'为存在
        }

        file.close();
    }

    //更新操作，对应update语句：
    //update tablename set attri_name2 = value2 where attri_name1 = value1

    void update(string attri_name1, string value1, string attri_name2, string value2)
    {
        string result;
        vector<int> search_result = search(Condition(attri_name1, '=', value1));

        int location = 1;
        Attribute attri;
        for (int i = 0; i < attributes.size(); i++)
        {
            if (attri_name2 == attributes[i].name)
            {
                attri = attributes[i];
                break;
            }
            location += attributes[i].size;
        }

        fstream file;
        file.open(name.c_str(), ios::out | ios::in | ios::binary);
        int size = get_record_size();
        for (int i = 0; i < search_result.size(); i++)
        {
            file.seekg(search_result[i] * size, ios::beg);
            file.seekp(location, ios::cur);
            result = string_to_binary(attri, value2);
            file.write(result.c_str(), result.size());
        }
        file.close();
    }

    void show_table()
    {
        for (int i = 0; i < attributes.size(); i++)
        {
            cout << attributes[i].name << "  ";
        }
        cout << endl;

        fstream file;
        file.open(name.c_str(), ios::in | ios::binary);

        char *temp = new char(get_record_size());

        while (file.peek() != EOF)
        {
            file.read(temp, 1);
            for (int i = 0; i < attributes.size(); i++)
            {
                file.read(temp, attributes[i].size);
                string data(temp, attributes[i].size);
                cout << binary_to_string(attributes[i], data) << " ";
            }
            cout << endl;
        }
        delete temp;
        file.close();
    }
};




#endif	/* TABLE_H */

