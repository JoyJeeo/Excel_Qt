#ifndef SITE_PART_H
#define SITE_PART_H
#include "operate_file.h"

// 芯片号和轮数
class SITE_PART
{
public:
    // 无参构造
    explicit SITE_PART();

    int get_Max_Site_Number();

    int get_Max_Part_Id();

// string代表SITE_NUM和PART_ID，vector中依次对应所测属性的数值【通过[下标]索引实现】
map<string,vector<int>> m_site_part; // 直接写m_在类中声明的好处是，对象可以通过m_的代码提示直接查看到public中，对外开放的所有可用属性
string SITE_NUM = "SITE_NUM";
string PART_ID = "PART_ID";

};

#endif // SITE_PART_H
