/* 
 * File:   define.h
 * Author: xiaose
 *
 * Created on 2012年10月17日, 下午5:14
 */

#ifndef DEFINE_H
#define	DEFINE_H
#define INTSIZE 4
#define NAMESIZE 20
#include <string>
#include <iostream>
#include "Attribute.h"
using namespace std;


enum DataType //枚举数据类型
{
    INT, CHAR, DOUBLE
};

struct Condition
{
    string table_name;
    Attribute attri;
    char operate;
};

#endif	/* DEFINE_H */

