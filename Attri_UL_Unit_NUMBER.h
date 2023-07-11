#ifndef ATTRI_UL_UNIT_NUMBER_H
#define ATTRI_UL_UNIT_NUMBER_H
#include "operate_file.h"
//#include "ul_unit_number.h"
// 专门存储行数据单元的结构体
struct UL_Unit_NUMBER{
    UL_Unit_NUMBER(){};
    UL_Unit_NUMBER(string unit,double limitL,double limitU)
                :m_Unit(unit),m_LimitL(limitL),m_LimitU(limitU){}
    string m_Unit;
    double m_LimitL;
    double m_LimitU;
};
// 所有属性的最大、最小值和属性
// 只定义结构，而不定义对象
class Attri_UL_Unit_NUMBER
{
public:
    // 无参构造
    explicit Attri_UL_Unit_NUMBER();

// string代表Limit的UL，vector中依次对应所测属性的数值【通过[下标]索引实现】
// 1. 这里将ul和unit耦合在一起，但这并不是一种耦合，因为ul与unit之间有直接的关系，大部分应用场景需要他们同时出现【有关系】
// 2. units作为单位的容器，不需要使用map的方式来占用更多的空间来实现【从效率的角度，不需要unit与ul使用相同的容器进行存储】【有效率】
// 3. 由于attri与uul有直接的对应关系,因此应该使用map的形式将attri与uul进行存储

//map<string,vector<double>> m_uls;
//vector<string> m_units; // Unit
const string m_Unit = "Unit";
const string m_LimitL = "LimitL";
const string m_LimitU = "LimitU";
map<string,UL_Unit_NUMBER> m_attri_uuls; // 使用attri -> uul的方式进行 行对应

};

#endif // ATTRI_UL_UNIT_NUMBER_H
