#ifndef UNIT_UL_H
#define UNIT_UL_H

#include <string>
#include <QDebug>
using namespace std;

// 专门存储行数据单元的结构体
struct Unit_Ul{
    explicit Unit_Ul(){};
    Unit_Ul(string unit,double limitL,double limitU)
                :m_Unit(unit),m_LimitL(limitL),m_LimitU(limitU){}
    string m_Unit;
    double m_LimitL;
    double m_LimitU;
};

#endif // UNIT_UL_H
