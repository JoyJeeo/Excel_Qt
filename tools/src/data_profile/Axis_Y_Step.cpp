#include "tools/include/data_profile/Axis_Y_Step.h"

#include <fstream>
#include <QDebug>
#include <iostream>
#include <sstream>
#include <QCoreApplication>

Axis_Y_Step::Axis_Y_Step()
{
    try {
        // 生成该对象时，就需要构造其环境
        total_task();


    } catch (...) {
        qDebug() << "Axis_Y_Step::Axis_Y_Step";
        throw ;
    }
}

Axis_Y_Step::~Axis_Y_Step()
{

}

map<string, double> Axis_Y_Step::get_steps() noexcept
{
    return m_steps;
}


ifstream Axis_Y_Step::input_file_open(const string& input_File_path)
{
    /*打开输入文件*/
    try {
        ifstream ifs;
        ifs.open(input_File_path);
        if(!ifs.is_open())
        {
            cerr<<"step file open fail"<<endl;
            exit(1);
        }
        return ifs;

    } catch (...) {
        qDebug() << "Axis_Y_Step::input_file_open";
        throw;
    }
}

void Axis_Y_Step::profile_step_file_path() noexcept
{
    step_file_path = QCoreApplication::applicationDirPath() + "/" + step_file_name;
}

void Axis_Y_Step::tackle_file(const ifstream& ifs)
{
    try {
        vector<vector<string>> all_array = tackle_file_get_all(ifs);
        profile_steps(all_array);

    } catch (...) {
        qDebug() << "Axis_Y_Step::tackle_file";
        throw ;
    }
}

vector<vector<string>>
Axis_Y_Step::tackle_file_get_all(const ifstream& ifs)
{
    /*将整个csv表格读入程序中*/
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

        return all_array;

    } catch (...) {
        qDebug() << "Axis_Y_Step::tackle_file_get_all";
        throw;
    }
}

void Axis_Y_Step::profile_steps(const vector<vector<string> > &all_array)
{
    try {
        int pro_col = 0;
        int step_col = 4;

        // 收获数据
        int begin_row = 1;
        for(size_t i = begin_row;i < all_array.size(); i++)
        {
            // 保证每个项目都有step【！！！】
            m_steps.insert(make_pair(all_array[i][pro_col],stod(all_array[i][step_col])));
        }

    } catch (...) {
        qDebug() << "Axis_Y_Step::profile_steps";
        throw ;
    }
}

void Axis_Y_Step::total_task()
{
    try {
        // 获取step文件路径
        profile_step_file_path();
//        qDebug() << step_file_path;
        // 打开step文件 如果打开失败，说明step文件可能不存在
        ifstream ifs = input_file_open(step_file_path.toStdString());

        // 提取文件内容
        tackle_file(ifs);

        ifs.close();


    } catch (...) {
        qDebug() << "Axis_Y_Step::total_task";
        throw ;
    }
}


