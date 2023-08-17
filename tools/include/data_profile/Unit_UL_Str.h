#ifndef UNIT_UL_STR_H
#define UNIT_UL_STR_H

#include <string>
#include <QDebug>
using namespace std;

// 专门存储行数据单元的结构体
struct Unit_UL_Str{
    explicit Unit_UL_Str(){};
    Unit_UL_Str(string unit,string limitL,string limitU)
                :m_Unit(unit),m_LimitL(limitL),m_LimitU(limitU){}

    bool operator!=(Unit_UL_Str& uul);
    bool operator==(Unit_UL_Str& uul);

    explicit operator string();

    string m_Unit;
    string m_LimitL;
    string m_LimitU;

private:
    // 将LimitL和LimitU的小数点后无效0去除
    void tackle_ul(Unit_UL_Str& uul);
    string profile_str_num(const string& num);
};

#endif // UNIT_UL_STR_H
