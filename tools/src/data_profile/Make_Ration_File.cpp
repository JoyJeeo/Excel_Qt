#include "tools/include/data_profile/Make_Ration_File.h"

#include <QDebug>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QCoreApplication>

Make_Ration_File::Make_Ration_File()
{
    try {
        profile_output_file_path();

    } catch (...) {
        qDebug() << "Make_Ration_File::Make_Ration_File";
        throw;
    }
}

Make_Ration_File::~Make_Ration_File()
{

}

const string Make_Ration_File::make_ration_file(const string& timc_file_path)
{
    /*
        功能：
            将读入的数据，分析生成ration中所需要的数据，并返回
    */
    try {
        // 初始化timc_datas
        init_timc_datas(timc_file_path);

        // 指挥初始化类环境
        director_indexs();

        init_ration_datas();

        build_ration_datas();


        // 保存ration_datas进入ration文件中
        save_datas();

        return ration_file_path.toStdString();

    } catch (...) {
        qDebug() << "Make_Ration_File::make_ration_file";
        throw;
    }
}

bool Make_Ration_File::clear_trash_datas()
{
    try {
        if(is_solo_time)
        {
            ifstream ifs = input_file_open(ration_file_path.toStdString());
            vector<vector<string>> all_array = tackle_file_get_all(ifs);
            ifs.close();

            // 获取真实的ration data
            vector<vector<string>> ans(all_array.begin(),all_array.begin() + trash_dex);
            ration_datas = ans;
            save_datas();

        }

        return is_solo_time;

    } catch (...) {
        qDebug() << "Make_Ration_File::clear_trash_datas";
        throw;
    }
}

ifstream Make_Ration_File::input_file_open(const string &input_file_path)
{
    try {
        ifstream ifs;
        ifs.open(input_file_path);
        if(!ifs.is_open())
        {
            cerr<<"ration file open fail"<<endl;
            exit(1);
        }
        return ifs; // 被移交内存

    } catch (...) {
        qDebug() << "Make_Ration_File::input_file_open";
        throw;
    }
}

ofstream Make_Ration_File::output_file_open(const string &out_file_path)
{
    try {
        ofstream ofs;
        ofs.open(out_file_path,ios::out | ios::trunc);
        if(!ofs.is_open())
        {
            cerr<<"please close the ration.csv, open fail"<<endl;
            exit(1);
        }

        return ofs;

    } catch (...) {
        qDebug() << "Make_Ration_File::output_file_open";
        throw;
    }
}

void Make_Ration_File::init_timc_datas(const string &timc_file_path)
{
    try {
        // 初始化timc_datas
        ifstream ifs = input_file_open(timc_file_path);
        timc_datas = tackle_file_get_all(ifs);
        ifs.close();

    } catch (...) {
        qDebug() << "Make_Ration_File::init_timc_datas";
        throw;
    }
}

void Make_Ration_File::init_ration_datas()
{
    try {
        // 初始化ration_datas
        // 计算ration数据存储容器的初始化大小
        int row_len = timc_datas.size() - (end_T0_body_begin_other_body - end_head_begin_T0_body );
        int col_len = timc_datas[0].size();
        // 以空字符串为初始化
        ration_datas = vector<vector<string>>(row_len,vector<string>(col_len,""));

    } catch (...) {
        qDebug() << "Make_Ration_File::init_ration_datas";
        throw;
    }
}

void Make_Ration_File::build_ration_datas()
{
    try {
        // 获取并修正timc文件的头数据
        profile_ration_head();

        // 移动T0后的数据到前面
        move_other_data();

        // 修改数据区的数据，转换为变动率
        update_valid_datas();

        // 判断ration中是否只有一个时刻
        if((is_solo_time = is_solo_part()) == true)
            copy_solo_part();

    } catch (...) {
        qDebug() << "Make_Ration_File::build_ration_datas";
        throw;
    }
}

void Make_Ration_File::save_datas()
{
    try {
        // 获取文件输出流
        ofstream ofs = output_file_open(ration_file_path.toStdString());

        // 将时刻数据内容写出
        for(size_t i = 0;i < ration_datas.size(); i++)
        {
            for(size_t j = 0;j < ration_datas[i].size();j++)
            {
                ofs<<ration_datas[i][j]<<",";
            }
            ofs<<endl;
        }

        ofs.close();

    } catch (...) {
        qDebug() << "Make_Ration_File::save_datas";
        throw;
    }
}

vector<vector<string>> Make_Ration_File::tackle_file_get_all(const ifstream &ifs)
{
    try {
        string s;
        vector<vector<string>> all_array;
        vector<string> arrays;
        string array;
        ifstream& t_ifs = const_cast<ifstream&>(ifs);
        while(getline(t_ifs,s))
        {
            istringstream sin(s);

            while(getline(sin,array,','))
            {
                // cout<<array.size()<<" ";
                arrays.push_back(array);
            }
            // cout<<endl;
            all_array.push_back(arrays);
            arrays.clear();
        }
//        test_datas(all_array);

        return all_array;

    } catch (...) {
        qDebug() << "Make_Ration_File::tackle_file_get_all";
        throw;
    }
}

void Make_Ration_File::profile_end_head_begin_T0_body()
{
    try {
        for(size_t i = 0;i < timc_datas.size();i++)
        {
            if(timc_datas[i][0] == this->div_str || timc_datas[i][0].size() == 0 ||
                    timc_datas[i].size() == 0)
            {
                end_head_begin_T0_body = i;
                end_head_begin_T0_body++;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_end_head_begin_T0_body";
        throw;
    }
}

void Make_Ration_File::profile_end_T0_body_begin_other_body()
{
    try {
        // 标记初始T0的数值
        int attri_col = 0;
        const string tag = timc_datas[end_head_begin_T0_body][attri_col];
        for(size_t i = end_head_begin_T0_body + 1;i < timc_datas.size();i++)
        {
            if(timc_datas[i][attri_col] == tag)
            {
                end_T0_body_begin_other_body = i;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_end_T0_body_begin_other_body";
        throw;
    }
}

void Make_Ration_File::profile_valid_col()
{
    try {
        // 有效列开始位置的前一位
        const string tag = "TEST_NUM"; // 不是【SITE_NUM】
        for(size_t row = 0,col = 0;col < timc_datas[row].size();col++)
        {
            if(timc_datas[row][col] == tag)
            {
                valid_col = col;
                valid_col++;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_valid_col";
        throw;
    }
}

void Make_Ration_File::profile_ration_head()
{
    try {
        size_t attri_row = 0,null_row = end_head_begin_T0_body - 1;

        // 读取数据头
        for(size_t i = 0;i < end_head_begin_T0_body;i++)
        {
            if(i == attri_row || i == null_row)
            {
                ration_datas[i] = timc_datas[i];
                continue;
            }

            for(size_t j = 0;j < timc_datas[i].size();j++)
            {
                if(j >= valid_col)
                {
                    switch (i) {
                        case 1 : // unit_row
                            ration_datas[i][j] = "%";
                            break;
                        case 2 : // limitL_row
                            ration_datas[i][j] = "-10";
                            break;
                        case 3: // limitU_row
                            ration_datas[i][j] = "10";
                            break;
                    }
                    continue;
                }
                ration_datas[i][j] = timc_datas[i][j];
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_ration_head";
        throw;
    }
}

void Make_Ration_File::move_other_data()
{
    try {
        // 设置前排数据索引和移动数据索引的开始位置
        size_t pre_index = end_head_begin_T0_body;
        size_t move_index = end_T0_body_begin_other_body;

        // 搬运和修改部分数据
        for(;move_index < timc_datas.size() && pre_index < timc_datas.size();
            move_index++,pre_index++)
        {
            for(size_t j = 0;j < valid_col;j++)
            {
                // 从timc向ration搬运其余数据
                ration_datas[pre_index][j] = timc_datas[move_index][j];
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::move_other_data";
        throw;
    }
}

void Make_Ration_File::update_valid_datas()
{
    try {
        // 分析并获取ration文件主数据
        // 对比数据T0数据区
        // 本质上类似于在"填表格"

        // 计算T0所在row与其开始行的偏移量【让后面的计算与T0的行偏移同步】【获取不同时刻】
        int T0_offset = 0;
        // 计算两个时刻之间的间隔数【其后每个时刻之间计算都以这个间隔为基础】【相同时刻的跨度】
        int interval_n = end_T0_body_begin_other_body - end_head_begin_T0_body;

        // 计算有效数据
        for(size_t row_T0 = end_head_begin_T0_body;row_T0 < end_T0_body_begin_other_body;
            row_T0++,T0_offset++) // 将时刻的no偏移,同步到其他时刻上
        {
            for(size_t col = valid_col;col < timc_datas[row_T0].size();col++)
            {
                for(size_t row_other = end_T0_body_begin_other_body + T0_offset,
                    // timc计算结果存入ration的结果中
                    row_ration_record = end_head_begin_T0_body + T0_offset; // row_ration_record既有timc的属性，又有ration的属性
                    row_other < timc_datas.size();
                    row_other += interval_n,row_ration_record += interval_n)
                {
                    // 【排除所有不应该被用来计算的情况】
                    // 保证两个都有数值【任何一方没有出现数值，都认为是不能被计算】
                    if(timc_datas[row_T0][col].size() == 0 || timc_datas[row_other][col].size() == 0)
                        continue;

                    double T0 = stod(timc_datas[row_T0][col]);
                    double T1 = stod(timc_datas[row_other][col]);
                    // 如果分母为0
                    if(T0 == 0.0)
                        continue;

                    // 【可以被计算的情况】
                    // 获取算法的计算结果
                    double ans_data = algorithm_ration_data(T1,T0);

                    // 将double数据转换为string字符串存储[√]
                    // 将timc的计算结果 -> ration结果中存储
                    ration_datas[row_ration_record][col] = to_string(ans_data);
                }
            }
        }
    } catch (...) {
        qDebug() << "Make_Ration_File::update_valid_datas";
        throw;
    }
}

double Make_Ration_File::algorithm_ration_data(double T1, double T0)
{
    try {

        // 按照%进行比较的
        double ans = ((T1 - T0) / T0) * 100;

        return ans;

    } catch (...) {
        qDebug() << "Make_Ration_File::algorithm_ration_data";
        throw;
    }
}

bool Make_Ration_File::is_solo_part()
{
    try {
        const int part_col = 1;
        // 获取第一个part
        string tag = ration_datas[end_head_begin_T0_body][part_col];

        for(size_t i = end_head_begin_T0_body;i < ration_datas.size();i++)
        {
            if(ration_datas[i][part_col] != tag) return false;
        }

        return true;

    } catch (...) {
        qDebug() << "Make_Ration_File::is_solo_part";
        throw;
    }
}

void Make_Ration_File::copy_solo_part()
{
    try {
        trash_dex = ration_datas.size();
        const int part_col = 1;

        for(size_t i = end_head_begin_T0_body;i < trash_dex;i++)
        {
            auto temp = ration_datas[i];
            temp[part_col] += " ";
            ration_datas.push_back(temp);
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::copy_solo_part";
        throw;
    }
}

void Make_Ration_File::profile_output_file_path() noexcept
{
    ration_file_path = QCoreApplication::applicationDirPath() +
            "/" + ration_file_name;
}


void Make_Ration_File::director_indexs()
{
    /*
        功能：
            指挥有顺序关系的函数按照指定顺序执行【人为指定】
    */
    try {
        profile_end_head_begin_T0_body();
        profile_end_T0_body_begin_other_body();
        profile_valid_col();

    } catch (...) {
        qDebug() << "Make_Ration_File::director_indexs";
        throw;
    }
}



