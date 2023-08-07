#include "tools/include/data_profile/Scatter_Site_X_Part.h"


Scatter_Site_X_Part::Scatter_Site_X_Part()
{
    m_attri_site_part.insert(make_pair(SITE_NUM,vector<string>()));
    m_attri_site_part.insert(make_pair(PART_ID,vector<string>()));
}

vector<string> Scatter_Site_X_Part::get_site_list()
{
    return m_attri_site_part[SITE_NUM];
}

vector<string> Scatter_Site_X_Part::get_part_list()
{
    return m_attri_site_part[PART_ID];
}

vector<string> Scatter_Site_X_Part::get_scatter_string_site()
{
    try {
        vector<string> scatter_str_site;
        vector<string> site_list = get_site_list();

        for(string site : site_list)
        {
            // 迭代器本身也在更新，因此必须这样动态调用迭代器
            if(find(scatter_str_site.begin(),scatter_str_site.end(),site) == scatter_str_site.end())
            {
                scatter_str_site.push_back(site);
            }
        }

        // site默认就是有序存在的
        return scatter_str_site;

    } catch (...) {
        qDebug() << "Scatter_Site_X_Part::get_scatter_string_site";
        throw;
    }
}

vector<int> Scatter_Site_X_Part::get_scatter_int_site()
{
    /*
        说明： 当site为int类型时，可能需要使用该函数
    */
    try {
        vector<int> scatter_int_site;
        vector<string> site_list = get_site_list();

        for(string site : site_list)
        {
            int val = stoi(site);
            if(find(scatter_int_site.begin(),scatter_int_site.end(),val) == scatter_int_site.end())
            {
                scatter_int_site.push_back(val);
            }
        }

        return scatter_int_site;

    } catch (...) {
        qDebug() << "Scatter_Site_X_Part::get_scatter_string_site";
        throw;
    }
}

vector<int> Scatter_Site_X_Part::get_x_part()
{
    try {
        vector<int> parts;
        vector<string> part_list = get_part_list();

        for(string part_str : part_list)
        {
            int part_int = stoi(part_str);
            if(find(parts.begin(),parts.end(),part_int) == parts.end())
            {
                parts.push_back(part_int);
            }
        }

        // part本质上可以不用对数据进行排序，但是为了不出现疑惑，这里选择对其进行排序
        sort(parts.begin(),parts.end()); // 从小到大排序

        return parts;

    } catch (...) {
        qDebug() << "Scatter_Site_X_Part::get_x_part";
        throw;
    }
}
