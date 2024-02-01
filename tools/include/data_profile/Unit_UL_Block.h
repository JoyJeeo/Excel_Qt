#ifndef UNIT_UL_BLOCK_H
#define UNIT_UL_BLOCK_H

#include <string>
#include <map>
#include "Unit_UL_Str.h"
#include <QDebug>

class Unit_UL_Block
{
public:
    // 无参构造
    explicit Unit_UL_Block(){}

const string m_Unit = "Unit";
const string m_LimitL = "LimitL";
const string m_LimitU = "LimitU";
map<string,Unit_UL_Str> m_attri_uuls;
vector<string> m_attri_list;

};

#endif // UNIT_UL_BLOCK_H
