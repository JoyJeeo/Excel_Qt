#include "tools/include/data_profile/SITE_PART.h"

// 无参构造
Site_Part::Site_Part()
{
    try {
        // 初始化m_site_part
        m_site_part.insert(make_pair(SITE_NUM,vector<int>()));
        m_site_part.insert(make_pair(PART_ID,vector<int>()));

        // 初始化m_time_site_part
        m_time_site_part.insert(make_pair(SITE_NUM,vector<pair<string,int>>()));
        m_time_site_part.insert(make_pair(PART_ID,vector<pair<string,int>>()));

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

vector<int> Site_Part::get_Scatter_Site_Number()
{
    /*
        功能：
            返回的数据，下标从0开始，内部存储的都为有效的site编号，
            site编号按照从小到大的顺序依次排布
    */
    try {
        // 均使用临时变量，运行后释放，尽量使用最小内存
        int max_site_number = get_Max_Site_Number();
        // 用列来代表site的编号，判断该site编号是否出现过
        vector<bool> vis(max_site_number+1,false);
        // 获取site整体数字列表用于遍历
        vector<int> site_list = get_site_list();

        // 遍历site_list
        for(int site : site_list)
        {
            vis[site] = true;
        }

        // 将存在的site压入ans中，以最小内存作为数据返回
        vector<int> ans;
        for(size_t site = 0;site < vis.size();site++)
        {
            if(vis[site])
            {
                ans.push_back(site);
            }
        }

        return ans;


    } catch (...) {
        qDebug() << "Site_Part::get_Scatter_Site_Number";
        throw ;
    }

}

vector<string> Site_Part::get_Scatter_Time_Site_Number()
{
    /*
        功能：
            将成员m_time_site_part中的内容，进一步优化，从而获取需要的数据形式
            【相当于，将原数据读入到对象中(这个对象作为中间对象)，通过类中的方法，对这些'源'数据进一步进行优化，
            返回我们需要的数据结构形式】
        返回：
            返回离散且唯一的time_site数据

    */
    try {
        // 获取site的列表
        vector<pair<string,int>> time_site_list = get_time_site_list();
        // 用来判断是否已经存在
        map<string,bool> vis;
        // 利用map的唯一性，来使得存储的site也具有唯一性
        vector<string> ans;

        // 遍历time_site_list
        for(pair<string,int> time_site : time_site_list)
        {
            // 如果vis中不存在，则添加
            if(vis.find(time_site.first) == vis.end())
            {
                ans.push_back(time_site.first);
                vis.insert(make_pair(time_site.first,true));
            }
        }

        return ans;

    } catch (...) {
        qDebug() << "Site_Part::get_Scatter_Time_Site_Number";
        throw;
    }
}

vector<pair<string,int>> Site_Part::get_time_site_list() noexcept
{
    return this->m_time_site_part[this->SITE_NUM];
}

int Site_Part::get_Max_Time_Part_Id()
{
    try {
        vector<pair<string,int>> v = this->m_time_site_part[PART_ID];
        int ans_Number = -1;
        for(auto x : v)
        {
            if(ans_Number < x.second)ans_Number = x.second;
        }
        return ans_Number;

    } catch (...) {
        qDebug() << "Site_Part::get_Max_Time_Part_Id";
        throw;
    }
}

vector<int> Site_Part::get_site_list() noexcept
{
    // 返回的是site列表数据，而不是所谓离散的site_number
    return this->m_site_part[this->SITE_NUM];
}





























