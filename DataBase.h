/* 
 * File:   DataBase.h
 * Author: xiaose
 *
 * Created on 2012年10月17日, 下午5:14
 */

#ifndef DATABASE_H
#define	DATABASE_H


#include <memory.h>
#include "Table.h"
#include "Condition.h"

class DataBase
{
public:
    vector<Table> tables; //表集合
    Table temp_table; //临时表

    DataBase()
    {
        read_table();
        temp_table.name = "temp";
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

        char name[NAMESIZE];
        int key_num;
        int number;
        DataType type;
        int type_size;

        char attri_name[NAMESIZE];

        while (file.peek() != EOF)
        {
            Table table;
            file.read(name, NAMESIZE); //读取表名
            table.name.append(name);

            file.read((char *) &key_num, INTSIZE);
            table.key_num = key_num;

            file.read((char *) &number, INTSIZE);

            for (int i = 0; i < number; i++)
            {
                file.read(attri_name, NAMESIZE);
                file.read((char *) &type, sizeof (DataType));
                file.read((char *) &type_size, INTSIZE);

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

        file.write(table_name.c_str(), NAMESIZE); //写入表名
        file.write((char *) &key_num, INTSIZE); //写入主键数
        int number = attributes.size();
        file.write((char *) &number, INTSIZE); //写入属性数量
        for (vector<Attribute>::iterator p = attributes.begin(); p != attributes.end(); p++)
        {

            file.write(p->name.c_str(), NAMESIZE);
            file.write((char *) &(p->type), sizeof (DataType));
            file.write((char *) &(p->size), INTSIZE);
        }
        file.close();
    }

    //等值连接
    //sql:select * from name1,name2 where name1.attri1 = name2.attri2;
    //其中l1,l2为两表，参与连接的元组所在行

    int Equi_Join(Table table1, Table table2, vector<int> l1, vector<int>l2)
    {

        //保证1号元素，元素大小最小
        if (l1.size() > l2.size())
        {
            Table ttemp = table1;
            vector<int> ltemp = l1;


            table1 = table2;
            l1 = l2;


            table2 = ttemp;
            l2 = ltemp;

        }


        fstream file1, file2, temp;
        file1.open(table1.name.c_str(), ios::in | ios::binary);
        file2.open(table2.name.c_str(), ios::in | ios::binary);
        temp.open("temp", ios::out | ios::binary);

        int newsize = table1.get_record_size() + table2.get_record_size() - 1;

        char * record = new char(newsize);


        for (int i = 0; i < l1.size(); i++)
        {
            file1.seekg(l1[i] * table1.get_record_size(), ios::beg);
            file1.read(record, table1.get_record_size());
            for (int j = 0; j < l2.size(); j++)
            {
                file2.seekg(l2[j] * table2.get_record_size() + 1, ios::beg);
                file2.read(record + table1.get_record_size(), table2.get_record_size() - 1);
                temp.write(record, newsize);
            }

        }

        file1.close();
        file2.close();
        temp.close();
        delete record;
        temp_table.attributes.clear();
        temp_table.attributes = table1.attributes;
        for (int i = 0; i < table2.attributes.size(); i++)
        {
            temp_table.attributes.push_back(table2.attributes[i]);
        }
    }

    int Equi_Join(Table table1, Table table2, vector<int> l1, vector<int>l2, Attribute attri1, Attribute attri2)
    {
        if (attri1.type != attri2.type || attri1.size != attri2.size)
            return 0;
        //保证1号元素，元素大小最小
        if (l1.size() > l2.size())
        {
            Table ttemp = table1;
            vector<int> ltemp = l1;
            Attribute atemp = attri1;

            table1 = table2;
            l1 = l2;
            attri1 = attri2;

            table2 = ttemp;
            l2 = ltemp;
            attri2 = atemp;
        }


        fstream file1, file2, temp;
        file1.open(table1.name.c_str(), ios::in | ios::binary);
        file2.open(table2.name.c_str(), ios::in | ios::binary);
        temp.open("temp", ios::out | ios::binary);

        //计算连接属性在各表中的位置
        int attri_p_1 = 1;
        for (int i = 0; i < table1.attributes.size(); i++)
        {
            if (table1.attributes[i].name == attri1.name)
                break;
            else
                attri_p_1 += table1.attributes[i].size;
        }

        int attri_p_2 = 1;
        for (int i = 0; i < table2.attributes.size(); i++)
        {
            if (table2.attributes[i].name == attri2.name)
                break;
            else
                attri_p_2 += table1.attributes[i].size;
        }

        int newsize = table1.get_record_size() + table2.get_record_size();
        newsize -= (attri1.size + 1);

        char * record = new char(newsize);



        char * temp1 = new char(attri1.size);
        char * temp2 = new char(attri2.size);

        for (int i = 0; i < l1.size(); i++)
        {
            file1.seekg(l1[i] * table1.get_record_size() + attri_p_1, ios::beg);
            file1.read(temp1, attri1.size);
            string data1(temp1, attri1.size);


            for (int j = 0; j < l2.size(); j++)
            {
                file2.seekg(l2[j] * table2.get_record_size() + attri_p_2, ios::beg);
                file2.read(temp2, attri2.size);
                string data2(temp2, attri2.size);
                //               
                if (data1 == data2)
                {
                    file1.seekg(l1[i] * table1.get_record_size() + 1);
                    record[0] = '1';
                    file1.read(record + 1, table1.get_record_size() - 1);

                    file2.seekg(l2[j] * table2.get_record_size() + 1);
                    int p = 0;
                    for (int i = 0; i < table2.attributes.size(); i++)
                    {
                        if (table2.attributes[i].name != attri2.name)
                        {
                            file2.read(record + table1.get_record_size() + p, table2.attributes[i].size);
                            p += table2.attributes[i].size;
                        }
                        else
                        {
                            file2.seekg(table2.attributes[i].size, ios::cur);
                        }
                    }
                    //               
                    temp.write(record, newsize);
                }

            }
        }
        file1.close();
        file2.close();
        temp.close();
        delete temp1;
        delete temp2;
        delete record;


        temp_table.attributes.clear();
        temp_table.attributes = table1.attributes;
        for (int i = 0; i < table2.attributes.size(); i++)
        {
            if (table2.attributes[i].name != attri2.name)
                temp_table.attributes.push_back(table2.attributes[i]);
        }

    }

    //投影操作，name为需要操作的文件名，attri为需要投影的那些属性

    void Projection(vector<Attribute> attri_full, vector<int> P)
    {
        fstream file1, file2;
        int i = 0;
        int j = 0;
        string result, temp;
        int location_v = 1;
        cout << "234422" << endl;
        int fullsize = 0;
        int size = 1;
        for (i = 0; i < attri_full.size(); i++)
            fullsize += attri_full[i].size;
        for (i = 0; i < P.size(); i++)
            size += attri_full[P[i]].size;
        file1.open("person", ios::in | ios::binary);
        file2.open("temp2", ios::out | ios::binary);
        vector<int>location_h;
        for (i = 0; i < location_h.size(); i++)
            cout << i << "abc" << endl;
        for (i = 0; i < P.size(); i++)
        {
            location_h.push_back(0);
            for (j = 0; j < P[i]; j++)
                location_h[i] += attri_full[j].size;
        }
        file1.seekg(location_v, ios::beg);
        while (file1.peek() != EOF)
        {
            result.append("1");
            for (i = 0; i < P.size(); i++)
            {
                char *temp = new char[attri_full[P[i]].size];
                file1.seekg(location_v + location_h[i], ios::beg);
                file1.read(temp, attri_full[P[i]].size);
                result.append(temp, attri_full[P[i]].size);
            }
            file2.write(result.c_str(), size);
            result.clear();
            location_v += fullsize;
            location_v++;
            file1.seekg(location_v, ios::beg);
        }
        file1.close();
        file2.close();
    }


    //选择操作
    //taboe_name,为涉及的表名，projection,为最后需要显示的属性，join为等值连接操作，condition为查询条件

    void Select(vector<string> table_name, vector<Table> projection, vector<Table> join, vector<Condition> condition)
    {


    }

    //显示temp的内容，attri为temp中包含的属性

    void Display(Table table)
    {
        temp_table.show_table();
    }


};




#endif	/* DATABASE_H */

