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
        // 获取文件读入流
        ifstream ifs = input_file_open(timc_file_path);
        // 读入容器中
        timc_datas = tackle_file_get_all(ifs);
        ifs.close();

        // 指挥初始化类环境
        director_indexs();

        // 计算ration数据存储容器的初始化大小
        int row_len = timc_datas.size() - (end_row_T0 - begin_row_T0 + 1);
        int col_len = timc_datas[0].size();
        // 以空字符串为初始化
        vector<vector<string>> ration_datas(row_len,vector<string>(col_len,""));

        // 读取数据头
        for(int i = 0;i < begin_row_T0;i++)
        {
            if(i == 0 || i == begin_row_T0 - 1)
            {
                ration_datas[i] = timc_datas[i];
                continue;
            }

            for(size_t j = 0;j < timc_datas[i].size();j++)
            {
                if(j >= begin_col)
                {
                    switch (i) {
                        case 1 :
                            ration_datas[i][j] = "%";
                            break;
                        case 2 :
                            ration_datas[i][j] = "-10";
                            break;
                        case 3:
                            ration_datas[i][j] = "10";
                            break;
                    }
                    continue;
                }
                ration_datas[i][j] = timc_datas[i][j];
            }
        }


        // 将some_time -> T0 的名称
//        string T0_site = timc_datas[begin_row_T0][0];
        // 搬运和修改部分数据
        for(size_t i = end_row_T0 + 1,counter = begin_row_T0;i < timc_datas.size();i++,counter++)
        {
            for(int j = 0;j < begin_col;j++)
            {
                // 获取some_time -> T0
//                if( j != 0)
//                {
//                    ration_datas[counter][j] = timc_datas[i][j];
//                    continue;
//                }
                // 获取其余数据
                ration_datas[counter][j] = timc_datas[i][j];
                        // + "->" + T0_site;
            }
        }

        // 分析并获取ration文件主数据
        // 对比数据T0数据区
        // 本质上类似于在"填表格"
        // 计算两个时刻之间的间隔数
        int hr_n = end_row_T0 - begin_row_T0 + 1;
        // 计算T0所在row与其开始行的偏移量【让后面的计算与T0的行偏移同步】
        int n = 0;
        for(int T0_row = begin_row_T0;T0_row <= end_row_T0;T0_row++,n++)
        {
            for(size_t col = begin_col;col < timc_datas[T0_row].size();col++)
            {
                for(size_t some_row = end_row_T0 + 1 + n,counter = begin_row_T0 + n // 每一列计算时，都需要进行更新
                    ;some_row < timc_datas.size();some_row += hr_n,counter += hr_n)
                {
                    // 保证两个都有数值【任何一方没有出现数值，都认为是不应该显示情况】
                    if(timc_datas[T0_row][col].size() == 0 || timc_datas[some_row][col].size() == 0)
                        continue;

                    double T0_data = stod(timc_datas[T0_row][col]);
                    double some_data = stod(timc_datas[some_row][col]);
                    // 算法分析生成的变动率数据 (未来 - 过去) / 过去
                    // 如果分母为0，则该位置数据，按照空来计 // 如果pre数据为0，则也认为是不需要进行显示的
                    if(T0_data == 0.0)
                    {
                        ration_datas[counter][col] = "";
                        continue;
                    }
                    // 按照%进行比较的
                    double ans_data = (some_data - T0_data) / T0_data * 100;
                    // 将double数据转换为string字符串[√]
                    ration_datas[counter][col] = to_string(ans_data);

//                    if(counter == 5 && col == 6)
//                    {
//                        qDebug() << "0";
//                    }
//                    qDebug() << T0_row << " " << some_row << " " << col;
                }
            }
        }


        // 保存ration_datas进入ration文件中
        save_datas(ration_datas);

        return ration_file_path.toStdString();

    } catch (...) {
        qDebug() << "Make_Ration_File::make_ration_file";
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

void Make_Ration_File::profile_begin_row_T0()
{
    try {
        for(size_t i = 0;i < timc_datas.size();i++)
        {
            if(timc_datas[i][0].size() == 0)
            {
                begin_row_T0 = i + 1;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_begin_row_T0";
        throw;
    }
}

void Make_Ration_File::profile_end_row_T0()
{
    try {
        // 标记初始T0的hr
        const string tag = timc_datas[begin_row_T0][0];
        for(size_t i = begin_row_T0 + 1;i < timc_datas.size();i++)
        {
            if(timc_datas[i][0] == tag)
            {
                end_row_T0 = i - 1;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_end_row_T0";
        throw;
    }
}

void Make_Ration_File::profile_begin_col()
{
    try {
        // 有效列开始位置的前一位
        const string tag = "TEST_NUM";
        for(size_t row = 0,col = 0;col < timc_datas[row].size();col++)
        {
            if(timc_datas[row][col] == tag)
            {
                begin_col = col + 1 < timc_datas[row].size() ? col + 1 : -1;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_begin_col";
        throw;
    }
}

void Make_Ration_File::profile_output_file_path() noexcept
{
    ration_file_path = QCoreApplication::applicationDirPath() +
            "/" + ration_file_name;
}

void Make_Ration_File::save_datas(const vector<vector<string>>& ration_datas)
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

void Make_Ration_File::director_indexs()
{
    /*
        功能：
            指挥有顺序关系的函数按照指定顺序执行【人为指定】
    */
    try {
        profile_begin_row_T0();
        profile_end_row_T0();
        profile_begin_col();

    } catch (...) {
        qDebug() << "Make_Ration_File::director_indexs";
        throw;
    }
}



