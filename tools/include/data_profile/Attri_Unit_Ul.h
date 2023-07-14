#ifndef ATTRI_UNIT_UL_H
#define ATTRI_UNIT_UL_H

#include <string>
#include <map>
#include "Unit_Ul.h"
#include <QDebug>

// 所有属性的最大、最小值和属性
// 只定义结构，而不定义对象
class Attri_Unit_Ul
{
public:
    // 无参构造
    explicit Attri_Unit_Ul(){}

// string代表Limit的UL，vector中依次对应所测属性的数值【通过[下标]索引实现】
// 1. 这里将ul和unit耦合在一起，但这并不是一种耦合，因为ul与unit之间有直接的关系，大部分应用场景需要他们同时出现【有关系】
// 2. units作为单位的容器，不需要使用map的方式来占用更多的空间来实现【从效率的角度，不需要unit与ul使用相同的容器进行存储】【有效率】
// 3. 由于attri与uul有直接的对应关系,因此应该使用map的形式将attri与uul进行存储

//map<string,vector<double>> m_uls;
//vector<string> m_units; // Unit
const string m_Unit = "Unit";
const string m_LimitL = "LimitL";
const string m_LimitU = "LimitU";
map<string,Unit_Ul> m_attri_uuls; // 使用attri -> uul的方式进行 行对应

};

#endif // ATTRI_UNIT_UL_H
