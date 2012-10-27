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

int str_value_fix(char *str)
{
	int start = find_char(0," ",0,str);

	if(str[start] == '\"')  //str type
	{
		start++;
		int end = strlen(str)-1;

		while(end >= 0)
		{
			if(str[end] == '\"')
			{
				end--;
				break;
			}
			else
				end--;
		}
		if(end+1 == start) //empty str
		{
			str[0] = '\0';
			return 1;
		}
		if(end < start)
			return -1;

		for(int i=start; i<=end; i++)
				str[i-start] = str[i];
		str[end+1] = '\0';
		return 1;
	}

	if(str[start] >= '0' && str[start] <= '9') //int type
	{
		int end = strlen(str)-1;

		while(end >= 0)
		{
			if(str[end] == ' ')
				end--;
			else
				break;
		}
		if(end < start)
			return -1;
		for(int i=start; i<=end; i++)
			if(!(str[i] >= '0'&& str[i] <= '9')) //int type
				return -1;
			else
				str[i-start] = str[i];

		return 1;
	}
}

void sql_insert(char *cmd, int start)
{
	char arg[1000];
	char table_name[1000];
	char attr_value[1000];
	char attr_name[1000];
	int end;
	int cnt = 0;
	int attr_cnt = 0;
	int no_table;
	vector<string> attr;
	vector<string> value;

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

    if (str_cmp(arg, "INTO", IGNORE))
    {
        if ((start = find_char(0, " ", end, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            return;
        }
        if ((end = find_char(1, " (", start, cmd)) == -1)
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
        //cout << table_name << endl;
		if((no_table = data.find_table(table_name)) == -1)
		{
			cout << "Error SQL: There is no " << table_name << " table in Database." << endl;
			return;
		}
		
	    if ((start = find_char(0, " ", end, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            return;
        }
		end = start;

		if(cmd[start] == '(')
		{
			while(1)
			{
				if ((start = find_char(0, "(, ", end, cmd)) == -1)
				{
					cout << "Error SQL" << endl;
					return;
				}
				if(cmd[start] == ')')
				{
					end = start;
					break;
				}
				attr_cnt++;
				if ((end = find_char(1, " ,)", start, cmd)) == -1)
				{
					cout << "Error SQL" << endl;
					return;
				}
				str_copy(attr_name, cmd, start, end);
				if(data.tables[no_table].get_attribute_by_name(attr_name) == -1)
				{
					cout << "Error SQL: There is no " << attr_name << " in " << table_name << endl;
					return;
				}
				attr.push_back(attr_name);
			//	cout << attr_cnt << " " << attr_name << endl;
			}
		}
		if(attr_cnt == 0)
		{
			attr_cnt = data.tables[no_table].attributes.size();
			attr = data.tables[no_table].get_all_attributes_name();
		}

		if ((start = find_char(0, " )", end, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            return;
        }
        if ((end = find_char(1, "( ", start, cmd)) == -1)
        {
            cout << "Error SQL" << endl;
            return;
        }
		
        str_copy(arg, cmd, start, end);

    	if (str_cmp(arg, "VALUES", IGNORE))
		{
			while (1)
			{
				if ((start = find_char(0, "(, ", end, cmd)) == -1)
				{
					cout << "Error SQL" << endl;
					return;
				}
				if(cmd[start] == ')')
					break;
				cnt++;
				if ((end = find_char(1, ",)", start, cmd)) == -1)
				{
					cout << "Error SQL" << endl;
					return;
				}
				str_copy(attr_value, cmd, start, end);
				if(str_value_fix(attr_value) == -1)
				{
					cout << "Error SQL: " << attr_value << " is not an invalid value." << endl;
					return;
				}
			//	cout << cnt << " " << attr_value << endl;
				value.push_back(attr_value);
            }
			if(cnt != attr_cnt)
			{
				cout << "Error SQL: the number of attributes != the number of values." << endl;
				return;
			}
			data.tables[no_table].Insert(attr,value);
		}
		else
		{
			cout << "Error SQL: It should be start as INSERT INTO table_name VALUES ..." << endl;
			return;
		}
	}
	else
	{
        cout << "Error SQL: It should be start as INSERT INTO ..." << endl;
        return;
	}
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
        if ((end = find_char(1, "( ", start, cmd)) == -1)
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

		if(data.find_table(table_name) != -1)
		{
			cout << "Error SQL: " << table_name << " is exist in database." << endl;
			return;
		}

        //cout << table_name << endl;
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

            //cout << cnt << " " << attr_name << " " << attr_type << " " << data_type_size << endl;

            if (cmd[start] == ')')
                break;
        }
        
        //cout << temp.size() << endl;
        data.create_table(table_name, 1, temp);
        return;
    }
    else
    {
        cout << "Error SQL: It should be start as CREATE TABLE ..." << endl;
        return;
    }
}

void sql_describe(char *cmd, int start)
{
    char arg[1000];
    char table_name[1000];
    int end;
	int no_table;

    if ((start = find_char(0, " ", start, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    if ((end = find_char(1, " ;", start, cmd)) == -1)
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
	if((no_table = data.find_table(table_name)) == -1)
	{
		cout << "Error SQL: There is no " << table_name << " table in Database." << endl;
		return;
	}
	data.read_table();
	data.tables[no_table].describe();
}

void sql_display(char *cmd, int start)
{
    char arg[1000];
    char table_name[1000];
    int end;
	int no_table;

    if ((start = find_char(0, " ", start, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    if ((end = find_char(1, " ;", start, cmd)) == -1)
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
	if((no_table = data.find_table(table_name)) == -1)
	{
		cout << "Error SQL: There is no " << table_name << " table in Database." << endl;
		return;
	}
	data.read_table();
	data.tables[no_table].show_table();
}

void sql_delete(char *cmd, int start)
{
    char arg[1000];
    char attr_signal[1000];
	char attr_value[1000];
    char attr_name[1000];
    char table_name[1000];
    int end;
	int no_table;

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

    if(!str_cmp(arg, "FROM", IGNORE))
    {
        cout << "Error SQL" << endl;
        return;
    }
  
    if ((start = find_char(0, " ", end, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    if ((end = find_char(1, " ", start, cmd)) == -1)
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
	if((no_table = data.find_table(table_name)) == -1)
	{
		cout << "Error SQL: There is no " << table_name << " table in Database." << endl;
		return;
	}

    if ((start = find_char(0, " ", end, cmd)) == -1)
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
    if(!str_cmp(arg, "WHERE", IGNORE))
    {
        cout << "Error SQL" << endl;
        return;
    }

  	if ((start = find_char(0, " ", end, cmd)) == -1)
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
    if ((end = find_char(1, " ", start, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    str_copy(attr_signal, cmd, start, end);

  	if ((start = find_char(0, " ", end, cmd)) == -1)
    {
        cout << "Error SQL" << endl;
        return;
    }
    str_copy(attr_value, cmd, start, strlen(cmd));

	cout << "1 " << table_name << " 11 " << attr_name << " 11 " << attr_signal << " 11 "<< attr_value << endl;
	data.tables[no_table].Delete(attr_name,attr_value);

}
void sql_update(char *cmd,int start)
{
   char seps[] = " ";
   char *token;
   char args[100][1000];
   int cnt=0;
   int no_table;

   token = strtok(cmd, seps );
   while( token != NULL )
   {
   	  strcpy(args[cnt++],token);
      //printf( "%d %s\n",cnt-1,args[cnt-1]);
      token = strtok( NULL, seps );
   }

   if((no_table = data.find_table(args[1])) == -1)
	{
		cout << "Error SQL: There is no " << args[1] << " table in Database." << endl;
		return;
	}
	data.tables[no_table].update(args[7],args[9],args[3],args[5]);
}
    //sql:select * from name1,name2 where name1.attri1 = name2.attri2;
void sql_select(char *cmd,int start)
{
   char seps[] = " ";
   char *token;
   char args[100][1000];
   int a_cnt=0;
   char name1[100];
   char attr1[100];
   char name2[100];
   char attr2[100];
   char value[100];
   int no_table;
   int no_attr;
   int cnt;
   vector<string> attri_name;
   vector<string> table_name;
   vector<Table> join;
   vector<Condition> conditon;

	char tmp_name[1000];

   token = strtok(cmd, seps );
   while( token != NULL )
   {
   	  strcpy(args[a_cnt++],token);
      printf( "%d %s\n",a_cnt-1,args[a_cnt-1]);
      token = strtok( NULL, seps );
   }

   if(args[1][0] != '*')
   {
   		cnt = 0;
   		token = strtok(args[1], ",");
   		while( token != NULL )
   		{
   	  		strcpy(tmp_name,token);
      		printf( "st %s\n",tmp_name);
			attri_name.push_back(tmp_name);
      		token = strtok( NULL, "," );
   		}
   }

	cnt = 0;
	token = strtok(args[3], ",");
	while( token != NULL )
	{
		strcpy(tmp_name,token);
      	printf( "ft %s\n",tmp_name);
		table_name.push_back(tmp_name);
      	token = strtok( NULL, ",." );
   	}
	for(int i=5;i<a_cnt;i+=2)
	{
		int sum = 0;
		for(int j=0;j<strlen(args[i]);j++)
			if(args[i][j] == '.')
				sum++;

		if(sum == 2)  //等值链接
		{
			token = strtok(args[i], ".");
			strcpy(name1,token);
      		token = strtok( NULL, "=" );
			strcpy(attr1,token);
			token = strtok(NULL, ".");
			strcpy(name2,token);
      		token = strtok( NULL, " " );
			strcpy(attr2,token);
			printf("dz %s %s %s %s\n",name1,attr1,name2,attr2);

			Table tmp_table_a,tmp_table_b;
			if((no_table = data.find_table(name1)) == -1)
			{
				cout << "Error SQL: There is no " << name1 << " table in Database." << endl;
				return;
			}
			if((no_attr = data.tables[no_table].find_attr(attr1)) == -1)
			{
				cout << "Error SQL: There is no " << attr1 << " attributes." << endl;
				return;
			}
			tmp_table_a = data.tables[no_table];
			tmp_table_a.attributes.clear();
			tmp_table_a.attributes.push_back(data.tables[no_table].attributes[no_attr]);

			if((no_table = data.find_table(name2)) == -1)
			{
				cout << "Error SQL: There is no " << name2 << " table in Database." << endl;
				return;
			}
			if((no_attr = data.tables[no_table].find_attr(attr2)) == -1)
			{
				cout << "Error SQL: There is no " << attr2 << " attributes." << endl;
				return;
			}
			tmp_table_b = data.tables[no_table];
			tmp_table_b.attributes.clear();
			tmp_table_b.attributes.push_back(data.tables[no_table].attributes[no_attr]);

			join.push_back(tmp_table_a);
			join.push_back(tmp_table_b);
		}
		else
		{
			token = strtok(args[i], ".");
			strcpy(name1,token);
      		token = strtok( NULL, "=" );
			strcpy(attr1,token);
			token = strtok(NULL, " ");
			strcpy(value,token);
			printf("tj %s %s %s\n",name1,attr1,value);
			
			Condition tmp_con(name1,attr1,'=',value);
			conditon.push_back(tmp_con);
		}
	}
	data.Select(table_name,attri_name,join,conditon);
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
        else if(str_cmp(arg, "INSERT", IGNORE))
            sql_insert(cmd, end);
        else if(str_cmp(arg, "DESCRIBE", IGNORE))
            sql_describe(cmd, end);
        else if(str_cmp(arg, "DISPLAY", IGNORE))
            sql_display(cmd, end);
		else if(str_cmp(arg, "DELETE", IGNORE))
            sql_delete(cmd, end);
		else if(str_cmp(arg, "UPDATE", IGNORE))
            sql_update(cmd, end);
		else if(str_cmp(arg, "SELECT", IGNORE))
            sql_select(cmd, end);
        else
        {
            cout << "Error SQL" << endl;
            continue;
        }
    }
}

