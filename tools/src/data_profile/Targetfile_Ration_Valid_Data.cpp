#include "tools/include/data_profile/Targetfile_Ration_Valid_Data.h"


#include <iostream>
#include <sstream>

extern int NULL_Number;

Targetfile_Ration_Valid_Data::Targetfile_Ration_Valid_Data()
{

}

Targetfile_Ration_Valid_Data::~Targetfile_Ration_Valid_Data()
{

}

void Targetfile_Ration_Valid_Data::total_task(const string target_file_path)
{
    try {
        // 获取数据输入流
        ifstream ifs = input_file_open(target_file_path);

        // 将文件内容载入程序
        load_target_file(ifs);
        // 分析all_array的数据有效列 和 头体数据分割索引点
        profile_head_body_div();
        profile_valid_col();
        // 分析有效数据
        profile_valid_datas();
        // 分析所有属性
        profile_labels();

        ifs.close();

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::total_task";
        throw   ;
    }
}

pair<double, double> Targetfile_Ration_Valid_Data::get_attri_value_XI(const string &attri) noexcept
{
    vector<string> scatter_site = m_site_part.get_scatter_string_site();
    vector<string> scatter_part = m_site_part.get_scatter_string_part();

    double ans_min = INT_MAX;
    double ans_max = INT_MIN;
    // 获取属性对应的处理数据【在原本已经处理好的数据上进行遍历】
    map<string,map<string,double>> datas = m_series_data[attri];

    // 有数据时的正常情况
    for(size_t i = 0;i < scatter_site.size();i++)
    {
        string site = scatter_site[i];
        for(size_t j = 0;j < scatter_part.size();j++)
        {
            string part = scatter_part[j];
            ans_max = max(ans_max,datas[site][part]);
            // 过滤有效的数据进行min的计算【因为数据为NULL的数值标记为了INT_MIN】
            if(datas[site][part] == INT_MIN) continue;
            ans_min = min(ans_min,datas[site][part]);
        }
    }
    // 数据全为NULL时的特殊情况
    if(ans_min == INT_MAX && ans_max == INT_MIN)
    {
        ans_min = INT_MIN;
        ans_max = INT_MAX;
    }
    return make_pair(ans_min,ans_max);//[最小值,最大值]
}

pair<double, double> Targetfile_Ration_Valid_Data::get_attri_define_XI(const string &attri) noexcept
{
    return make_pair(m_uul.m_attri_uuls[attri].m_LimitL,m_uul.m_attri_uuls[attri].m_LimitU);
}

vector<vector<string> > Targetfile_Ration_Valid_Data::get_target_file_vec() noexcept
{
    return m_all_array;
}

vector<string> Targetfile_Ration_Valid_Data::get_scatter_site()
{
    try {
        return m_site_part.get_scatter_string_site();

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::get_scatter_site";
        throw;
    }
}

vector<string> Targetfile_Ration_Valid_Data::get_scatter_part()
{
    try {
        return m_site_part.get_scatter_string_part();

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::get_scatter_part";
        throw;
    }
}

map<string, int> Targetfile_Ration_Valid_Data::get_part_map() noexcept
{
    return m_part_map;
}

string Targetfile_Ration_Valid_Data::get_attri_unit(const string &attri) noexcept
{
    return m_uul.m_attri_uuls[attri].m_Unit;
}

map<string, map<string, map<string, double>>> Targetfile_Ration_Valid_Data::get_series_datas() noexcept
{
    return m_series_data;
}

const vector<string> Targetfile_Ration_Valid_Data::get_labels() noexcept
{
    return labels;
}

ifstream Targetfile_Ration_Valid_Data::input_file_open(const string &input_file_path)
{
    try {
        ifstream ifs;
        ifs.open(input_file_path);
        if(!ifs.is_open())
        {
            cerr<<"target file open fail"<<endl;
            exit(1);
        }
        return ifs; // 被移交内存

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::input_file_open";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::load_target_file(const ifstream &ifs)
{
    try {
        string s;
        vector<string> arrays;
        string array;
        ifstream& t_ifs = const_cast<ifstream&>(ifs);
        while(getline(t_ifs,s))
        {
            istringstream sin(s);

            while(getline(sin,array,','))
            {
                arrays.push_back(array);
            }
            m_all_array.push_back(arrays);
            arrays.clear();
        }

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::load_target_file";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_head_body_div()
{
    try {
        size_t& i = end_head_begin_body;
        for(i = 0;i < m_all_array.size();i++)
        {
            if(m_all_array[i][0] == this->div_attri) break;
        }

        // 空行的下一位为数据起始位，保证结尾为开区间'[)'
        i++;

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_head_body_div";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_valid_col()
{
    try {
        size_t row; // SITE_NUM所在行
        size_t& col = valid_col;

        for(row = 0,col = 0;col < m_all_array[row].size();col++)
        {
            // part_id一定为整数
            if(m_all_array[row][col] == this->div_col)
            {
                // 获得有效列
                col++;
                break;

            }
        }

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_valid_col";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_valid_datas()
{
    try {
        profile_site_part();
        profile_uul();
        profile_series_data();
        profile_part_map();

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_valid_datas";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_labels()
{
    try {

        for(size_t i = end_head_begin_body;i < m_all_array.size();i++)
        {
            this->labels.push_back(m_all_array[i][0]);
        }

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_labels";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_site_part()
{
    try {
        int attri_col = 0;

        for(size_t i = 0;i < 2;i++)
        {
            string attri = m_all_array[i][attri_col];

            for(size_t j = valid_col;j < m_all_array[i].size();j++)
            {
                m_site_part.m_attri_site_part[attri].push_back(m_all_array[i][j]);
            }
        }

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_site_part";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_uul()
{
    try {
        size_t uul_index = 0;

        for(size_t i = end_head_begin_body;i < m_all_array.size();i++)
        {
            string attri=m_all_array[i][uul_index]; // 存储属性名称
            string unit=""; // 初始化，有些属性可能并没有单位【这是合法的】
            double limitL=INT_MIN,limitU=INT_MAX; // 需要插入的data数据

            // 对列依次查找,到end_col_dex分界点时结束遍历
            for(size_t j = 0;j < valid_col;j++)
            {
                // unit定义,未定义都存储
                if(m_all_array[uul_index][j] == m_uul.m_Unit)
                    unit = m_all_array[i][j];
                // limitL 定义的存储其值,未定义的存储INT_MIN
                else if(m_all_array[uul_index][j] == m_uul.m_LimitL
                        && m_all_array[i][j].size() != 0)
                    limitL = stod(m_all_array[i][j]);
                // limitU 定义的存储其值,未定义的存储INT_MAX
                else if(m_all_array[uul_index][j] == m_uul.m_LimitU
                        && m_all_array[i][j].size() != 0)
                    limitU = stod(m_all_array[i][j]);
            }
            // 将attri -> uul插入
            m_uul.m_attri_uuls.insert(make_pair(attri,
                                            Unit_Ul(unit,limitL,limitU)));
        }
    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_uul";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_series_data()
{
    try {
        // 初始化
        vector<string> scatter_site = m_site_part.get_scatter_string_site();
        vector<string> scatter_part = m_site_part.get_scatter_string_part();
        map<string,map<string,double>> init_data;

        // 初始化init_data
        for(auto site : scatter_site)
        {
            for(auto part : scatter_part)
            {
                init_data[site][part] = NULL_Number;
            }
        }

        // 下标区
        // SITE_NUM在target_file表中的下标位置
        int site_dex = 0;
        // PART_ID在target_file表中的下标位置
        int part_dex = 1;
        // 属性所在lie
        int attri_col = 0;

        // 遍历填充m_series_datas
        for(size_t i = end_head_begin_body;i < m_all_array.size();i++)
        {
            // 用来记录该行的属性值
            string attri = m_all_array[i][attri_col];
            // 使用init_data，加速初始化
            map<string,map<string,double>> data = init_data;

            for(size_t j = valid_col; j < m_all_array[i].size();j++)
            {
                // 正式读取主数据群数据，填充map<int,vector<double>> data
                if(m_all_array[i][j] == "NULL")  continue;

                // 转化行列下标
                string site = m_all_array[site_dex][j];
                string part = m_all_array[part_dex][j];

                // 转换行列后，获取数据
                data[site][part] = stod(m_all_array[i][j]);
            }
            // 添加属性的主数据进map
            // 乱序存储 但可以通过labels vector进行查看检索
            m_series_data.insert(make_pair(attri,data));
        }

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_series_data";
        throw;
    }
}

void Targetfile_Ration_Valid_Data::profile_part_map()
{
    try {
        vector<string> part_str = get_scatter_part();

        for(size_t i = 0;i < part_str.size();i++)
        {
            int no = i + 1;
            // 获取横坐标的映射数据
            m_part_map.insert(make_pair(part_str[i],no));
        }



    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::profile_part_map";
        throw;
    }
}

size_t Targetfile_Ration_Valid_Data::get_row_index_by_attri(const string &attri)
{
    try {
        size_t i;
        for(i = 0;i < m_all_array.size();i++)
        {
            if(m_all_array[i][0] == attri)break;
        }
        return i;

    } catch (...) {
        qDebug() << "Targetfile_Ration_Valid_Data::get_row_index_by_attri";
        throw;
    }
}

