#define IGNORE true  //SQL 忽略大小写标志
#include <iostream>
using namespace std;
#include <vector>
int find_char(int find_type, char *find_char, int i, char *str)
{
    if (i >= strlen(str) || i < 0)
        return -1;

    if (find_char[0] == '\0')
    {
        cout << "find_char不能为空" << endl;
        return -1;
    }

    if (find_type == 0) //返回第一个不是find_char里的字符位置
    {
        while (str[i] != '\0')
        {
            int j = 0;
            while (find_char[j] != '\0')
                if (str[i] == find_char[j])
                    break;
                else
                    j++;
            if (find_char[j] == '\0')
                return i;

            i++;
        }
    }
    else //返回第一个是find_char里的字符位置
    {
        while (str[i] != '\0')
        {
            int j = 0;
            while (find_char[j] != '\0')
                if (str[i] == find_char[j])
                    return i;
                else
                    j++;
            i++;
        }
    }

    return -1;
}

void str_copy(char *aim, char *source, int start, int end)
{
    int j = 0;
    for (int i = start; i < end; i++)
    {
        aim[j] = source[i];
        j++;
    }
    aim[j] = '\0';
}

bool str_cmp(char *a, char *b, bool ignore)
{
    int len = strlen(a);
    char char_a, char_b;

    if (len != strlen(b))
        return false;

    while (len >= 0)
    {
        if (a[len] >= 'a' && a[len] <= 'z' && ignore)
            char_a = a[len] + 'A' - 'a';
        else
            char_a = a[len];

        if (b[len] >= 'a' && b[len] <= 'z' && ignore)
            char_b = b[len] + 'A' - 'a';
        else
            char_b = b[len];

        if (char_a != char_b)
            return false;
        len--;
    }
    return true;
}

int str_to_num(char *str, char *ignore_char)
{
    int num = 0;
    int i = 0;

    if (str[0] == '\0')
        return -1;

    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
            num = num * 10 + str[i] - '0';
        else
        {
            int j = 0;
            while (ignore_char[j] != '\0')
            {
                if (ignore_char[j] == str[i])
                    break;
                j++;
            }
            if (ignore_char[j] == '\0')
                return -1;
        }
        i++;
    }

    return num;
}

void sql_insert(char *cmd, int start)
{
}

void sql_create(char *cmd, int start)
{
    char arg[1000];
    char table_name[1000];
    char attr_name[1000];
    char attr_type[1000];
    char type_size[1000];
    int data_type_size;
    int end;
    DataBase data;
    vector<Attribute> temp;

    if ((start = find_char(0, " ", start, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    if ((end = find_char(1, " ", start, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    str_copy(arg, cmd, start, end);
    if (str_cmp(arg, "TABLE", IGNORE))
    {
        if ((start = find_char(0, " ", end, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            return;
        }
        if ((end = find_char(1, "(", start, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            return;
        }
        str_copy(table_name, cmd, start, end);
        if (!strcmp(table_name, ""))
        {
            cout << "Error SQL: Please input table_name" << endl;
            return;
        }
        cout << table_name << endl;
        int cnt = 0;
        while (1)
        {
            cnt++;
            if ((start = find_char(0, "( ,", end, cmd)) == -1)
            {
                cout << "Error SQL" << endl;
                return;
            }
            if ((end = find_char(1, " ", start, cmd)) == -1)
            {
                cout << "Error SQL" << endl;
                return;
            }
            str_copy(attr_name, cmd, start, end);
            if ((start = find_char(0, " ", end, cmd)) == -1)
            {
                cout << "Error SQL" << endl;
                return;
            }
            if ((end = find_char(1, "(), ", start, cmd)) == -1)
            {
                cout << "Error SQL" << endl;
                return;
            }
            str_copy(attr_type, cmd, start, end);
            if (str_cmp(attr_type, "CHAR", IGNORE))
            {
                if ((start = find_char(0, " (", end, cmd)) == -1)
                {
                    cout << "Error SQL" << endl;
                    return;
                }
                if ((end = find_char(1, ")", start, cmd)) == -1)
                {
                    cout << "Error SQL" << endl;
                    return;
                }
                str_copy(type_size, cmd, start, end);
                end++;
            }
            else if (str_cmp(attr_type, "INT", IGNORE))
                strcpy(type_size, "4");
            else
            {
                cout << "Error SQL: " << attr_type << " is an invalid datatype." << endl;
                return;
            }

            if ((data_type_size = str_to_num(type_size, " ")) == -1)
            {
                cout << "Error SQL: " << type_size << " is not a positive integer." << endl;
                return;
            }

            if ((start = find_char(0, " ,", end, cmd)) == -1)
            {
                cout << "Error SQL" << endl;
                return;
            }

            if (str_cmp(attr_type, "CHAR", IGNORE))
                temp.push_back(Attribute(attr_name, CHAR, data_type_size));
            if (str_cmp(attr_type, "INT", IGNORE))
                temp.push_back(Attribute(attr_name, INT, data_type_size));

            cout << cnt << " " << attr_name << " " << attr_type << " " << data_type_size << endl;

            if (cmd[start] == ')')
                break;
        }
        
        cout << temp.size() << endl;
        data.create_table(table_name, 1, temp);
        data.read_table();
        data.show_database();
        return;
    }
    else
    {
        cout << "Error SQL: It should be start as CREATE TABLE ..." << endl;
        return;
    }
}

void sql_input()
{
    char cmd[1000];
    char arg[1000];
    int start;
    int end;

    while (1)
    {
        gets(cmd);

        if (str_cmp(cmd, "EXIT", IGNORE))
            return;

        if ((start = find_char(0, " ", 0, cmd)) == -1)
            continue;
        if ((end = find_char(1, " ", start, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            continue;
        }
        str_copy(arg, cmd, start, end);

        if (str_cmp(arg, "CREATE", IGNORE))
            sql_create(cmd, end);
        else if (str_cmp(arg, "INSERT", IGNORE))
            sql_insert(cmd, end);
        else
        {
            cout << "Error SQL" << endl;
            continue;
        }
    }
}


