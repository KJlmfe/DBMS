enum DataType 	//枚举数据类型
{
    INT, CHAR, DOUBLE
};

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
