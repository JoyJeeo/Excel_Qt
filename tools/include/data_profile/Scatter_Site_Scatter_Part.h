#ifndef SCATTER_SITE_SCATTER_PART_H
#define SCATTER_SITE_SCATTER_PART_H

#include <map>
#include <string>
#include <vector>
#include <QDebug>
using namespace std;

class Scatter_Site_Scatter_Part
{
public:
    Scatter_Site_Scatter_Part();

    // 获取离散的site【根据需要进行获取】
    // string类型
    vector<string> get_scatter_string_site();

    // int类型
    vector<int> get_scatter_int_site();

    // 获取离散的part_id
    // int型
    vector<int> get_scatter_int_part();
    // string型
    vector<string> get_scatter_string_part();

    // 获取SITE_NUM的原数据列表
    vector<string> get_site_list();

    // PART_ID的原数据列表
    vector<string> get_part_list();

    map<string,vector<string>> m_attri_site_part;

    string SITE_NUM = "SITE_NUM";
    string PART_ID = "PART_ID";

//private:
//    void profile_scatter_string_site();
//    void profile_scatter_int_site();
//    void profile_x_part();

//    vector<string> m_scatter_str_site;
//    vector<int> m_scatter_int_site;
//    vector<int> m_parts;

};

#endif // SCATTER_SITE_SCATTER_PART_H
