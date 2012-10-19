/* 
 * File:   Condition.h
 * Author: xiaose
 *
 * Created on 2012年10月18日, 下午3:52
 */
#include <string>
using namespace std;
#ifndef CONDITION_H
#define	CONDITION_H
class Condition
{
public:
    string table_name;
    string attri_name;
    char operate;
    string value;
    Condition(string table_name,string attri_name,char operate,string value)
    {
        this->table_name = table_name;
        this->attri_name = attri_name;
        this->operate = operate;
        this->value = value;
    }
      Condition(string attri_name,char operate,string value)
    {
        this->table_name = table_name;
        this->attri_name = attri_name;
        this->operate = operate;
        this->value = value;
    }
};


#endif	/* CONDITION_H */

