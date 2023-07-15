#include "../../include/data_profile/SITE_PART.h"

// 无参构造
Site_Part::Site_Part()
{
    try {
        // 初始化m_site_part
        m_site_part.insert(make_pair(SITE_NUM,vector<int>()));
        m_site_part.insert(make_pair(PART_ID,vector<int>()));

    } catch (...) {
        qDebug() << "Site_Part::Site_Part";
        throw;
    }
}

int Site_Part::get_Max_Site_Number()
{
    try {
        vector<int> v = this->m_site_part[SITE_NUM];
        int ans_Number = -1;
        for(auto x : v)
        {
            if(ans_Number < x)ans_Number = x;
        }
        return ans_Number;

    } catch (...) {
        qDebug() << "Site_Part::get_Max_Site_Number";
        throw;
    }
}

int Site_Part::get_Max_Part_Id()
{
    try {
        vector<int> v = this->m_site_part[PART_ID];
        int ans_Number = -1;
        for(auto x : v)
        {
            if(ans_Number < x)ans_Number = x;
        }
        return ans_Number;

    } catch (...) {
        qDebug() << "Site_Part::get_Max_Part_Id";
        throw;
    }
}
