#include "tools/include/data_profile/Make_Ration_File.h"

#include <QDebug>

Make_Ration_File::Make_Ration_File(vector<vector<string> > time_datas)
{
    // 构造环境
    this->m_source_datas = time_datas;
    director_indexs();
}

vector<vector<string>> Make_Ration_File::make_ration_file()
{
    /*
        功能：
            将读入的数据，分析生成ration_file
    */
    try {
        // 计算ration数据存储容器的初始化大小
        int row_len = m_source_datas.size() - (end_row_T0 - begin_row_T0 + 1);
        int col_len = m_source_datas[0].size();

        vector<vector<string>> ration_datas(row_len,vector<string>(col_len));

        // 读取数据头
        for(int i = 0;i < begin_row_T0;i++)
        {
            ration_datas[i] = (m_source_datas[i]);
        }

        // 读取标记比较数据区
        for(size_t i = begin_row_T0,counter = 0;i < m_source_datas.size();i++)
        {
            for(int j = 0;j < begin_col;j++)
            {

            }
        }

        // 分析并获取ration文件主数据
        // 对比数据T0数据区
        // 本质上类似于在"填表格"
        for(int T0_row = begin_row_T0;T0_row <= end_row_T0;T0_row++)
        {
            for(size_t col = begin_col;col < m_source_datas[T0_row].size();col++)
            {
                for(size_t some_row = end_row_T0 + 1,counter = begin_row_T0 // 每一列计算时，都需要进行更新
                    ;some_row < m_source_datas.size();some_row++,counter++)
                {
                    double T0_data = stod(m_source_datas[T0_row][col]);
                    double some_data = stod(m_source_datas[some_row][col]);
                    ration_datas[counter][col] = (some_data - T0_data) / T0_data;
                }
            }
        }


    } catch (...) {
        qDebug() << "Make_Ration_File::make_ration_file";
        throw;
    }
}

void Make_Ration_File::profile_begin_row_T0()
{
    try {
        for(size_t i = 0;i < m_source_datas.size();i++)
        {
            if(m_source_datas[i][0].size() == 0)
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
        const string tag = m_source_datas[begin_row_T0][0];
        for(size_t i = begin_row_T0;i < m_source_datas.size();i++)
        {
            if(m_source_datas[i][0] != tag)
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
        for(size_t row = 0,col = 0;col < m_source_datas[row].size();col++)
        {
            // SITE_NUM不一定为整数
            if(is_Integer_Numeric(m_source_datas[row][col]) ||
                    is_Integer_Numeric(m_source_datas[row+1][col]))
            {
                begin_col = col;
                break;
            }
        }

    } catch (...) {
        qDebug() << "Make_Ration_File::profile_begin_col";
        throw;
    }
}

bool Make_Ration_File::is_Integer_Numeric(const string &str)
{
    /*
        通过判断str中是否有非0-9的数据，来判断str是否为一个存储数值的字符串

        该函数只能判断，str是否为一个非小数的数值数据
    */
    try {
        auto it = str.begin();
        while (it != str.end() && std::isdigit(*it)) {
            it++;
        }
        return !str.empty() && it == str.end();

    } catch (...) {
        qDebug() << "Make_Ration_File::is_Integer_Numeric";
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



