class DataBase
{
public:
    vector<Table> tables;  //表集合

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

};

