#ifndef SITE_PART_H
#define SITE_PART_H

#include <map>
#include <string>
#include <vector>
#include <QDebug>
using namespace std;

// 芯片号和轮数
class Site_Part
{
public:
    // 无参构造
    explicit Site_Part();

    int get_Max_Site_Number();

    int get_Max_Part_Id();

    // site_number更倾向于一种离散的数据关系，而不是连续的数据
    // 返回其离散数值，在使用意义上更有效，更能提高程序的运行效率，并且更加具有可扩展性
    vector<int> get_Scatter_Site_Number();

    vector<int> get_site_list() noexcept;


// string代表SITE_NUM和PART_ID，vector中依次对应所测属性的数值【通过[下标]索引实现】
map<string,vector<int>> m_site_part; // 直接写m_在类中声明的好处是，对象可以通过m_的代码提示直接查看到public中，对外开放的所有可用属性
string SITE_NUM = "SITE_NUM";
string PART_ID = "PART_ID";

};

#endif // SITE_PART_H
