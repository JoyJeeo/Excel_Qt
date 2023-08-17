#include "tools/include/data_profile/Unit_UL_Str.h"



bool Unit_UL_Str::operator!=(Unit_UL_Str &uul)
{
    tackle_ul(*this);
    tackle_ul(uul);

    if(uul.m_Unit != this->m_Unit ||
            uul.m_LimitL != this->m_LimitL ||
            uul.m_LimitU != this->m_LimitU)
        return true;

    return false;
}

bool Unit_UL_Str::operator==(Unit_UL_Str &uul)
{
    return !operator!=(uul);
}

void Unit_UL_Str::tackle_ul(Unit_UL_Str &uul)
{
    try {
        if(uul.m_LimitL != "")
        {
            uul.m_LimitL = profile_str_num(uul.m_LimitL);

        }
        if(uul.m_LimitU != "")
        {
            uul.m_LimitU = profile_str_num(uul.m_LimitU);

        }

    } catch (...) {
        qDebug() << "Unit_UL_Str::tackle_ul";
        throw;
    }
}

string Unit_UL_Str::profile_str_num(const string &num)
{
    try {
        // 保证num不为""空
        double d = stod(num);
        // 通过double中转一下，保证所有string的double都是"*.*"的结构
        string d_str = to_string(d);

        size_t dot = 0;
        size_t last_num = num.size() - 1;

        // 初始化dot
        for(;dot < num.size();dot++)
        {
            if(d_str[dot] == '.')break;
        }

        // 初始化dot_after_num
        for(;last_num > dot;last_num--)
        {
            if(d_str[last_num] != '0')break;
        }

        if(last_num != dot) last_num++;

        return d_str.substr(0,last_num);


    } catch (...) {
        qDebug() << "Unit_UL_Str::profile_str_num";
        throw;
    }
}

Unit_UL_Str::operator string()
{
    string str = "Unit: " + m_Unit +
            " LimitL: " + m_LimitL +
            " LimitU: " + m_LimitU;

    return str;
}
