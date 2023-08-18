#include "tools/include/data_profile/Make_Merge_File.h"

#include <QDebug>
#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <sstream>

Make_Merge_File::Make_Merge_File()
{
    try {
        profile_output_file_path();

    } catch (...) {
        qDebug() << "Make_Merge_File::Make_Merge_File";
        throw;
    }
}

Make_Merge_File::~Make_Merge_File()
{

}

const string Make_Merge_File::make_merge_file(const QStringList &file_paths)
{
    try {
        // 只有第一个文件才会读取头数据
        bool first_file_flage = true;

        // 遍历所有温度文件
        for(int i = 0; i < file_paths.size();i++)
        {
            // 送入单文件处理，进行处理
            tackle_single_file(file_paths[i],first_file_flage);

            // 拒绝获取头部数据
            if(i == 0) first_file_flage = false;
        }

        // 处理所有文件结束，将文件内容进行保存
        save_datas();

        // 判断是否需要展示warning
        if(test_plan.warning_flage())
            test_plan.warning_show();

        // 返回保存的temperature文件的路径
        return merge_file_path.toStdString();

    } catch (...) {
        qDebug() << "Make_Merge_File::make_merge_file";
        throw;
    }
}

ifstream Make_Merge_File::input_file_open(const string &input_file_path)
{
    try {
        ifstream ifs;
        ifs.open(input_file_path);
        if(!ifs.is_open())
        {
            cerr<<"temperature file open fail"<<endl;
            exit(1);
        }
        return ifs; // 被移交内存

    } catch (...) {
        qDebug() << "Make_Merge_File::input_file_open";
        throw;
    }
}

ofstream Make_Merge_File::output_file_open(const string &out_file_path)
{
    try {
        ofstream ofs;
        ofs.open(out_file_path,ios::out | ios::trunc);
        if(!ofs.is_open())
        {
            cerr<<"please close the temperatures.csv, open fail"<<endl;
            exit(1);
        }

        return ofs;

    } catch (...) {
        qDebug() << "Make_Merge_File::output_file_open";
        throw;
    }
}

void Make_Merge_File::tackle_single_file(const QString &file_path, bool first_file_flage)
{
    try {
        // 获取文件的输入流
        ifstream ifs = input_file_open(file_path.toStdString());

        // 将文件内容全部读入程序中
        vector<vector<string>> all_array = tackle_solo_file_get_all(ifs);

        // 关闭读入流
        ifs.close();

        // 获取目标数据区参数
        size_t rows_num = 0;
        size_t cols_num = 0;
        size_t target_data_index = 0;

        // 分析all_array，获取目标数据区的参数
        profile_target_data_params(all_array,rows_num,cols_num,target_data_index);

        // 获取目标数据区数据
        vector<vector<string>> target_array = tackle_solo_file_get_target(all_array,rows_num,
                                                                          cols_num,
                                                                          target_data_index);

        // 获取目标数据区中的分割点inde
        size_t end_head_begin_body = 0;

        // 分析目标数据的分割点下标，分析出头数据和体数据的位置
        profile_div_index(target_array,end_head_begin_body);

        // 头数据处理
        // 获取头数据
        vector<vector<string>> head_datas = get_head_datas(target_array,end_head_begin_body);
        // 分析是否头数据与test plan存在不符
        test_plan.warning_head(head_datas,file_path.toStdString());
        // 只有第一个文件会获取头数据
        if(first_file_flage)
        {
            // 将头数据加载进总容器中
            load_datas(head_datas);
        }

        // 体数据处理
        vector<vector<string>> body_datas = get_body_datas(target_array,end_head_begin_body);
        // 修改body_datas中的数据
        vector<vector<string>> update_body = update_body_datas(body_datas);
        // 加载进容器中
        load_datas(update_body);

    } catch (...) {
        qDebug() << "Make_Merge_File::tackle_single_file";
        throw;
    }
}

vector<vector<string>> Make_Merge_File::tackle_solo_file_get_all(const ifstream &ifs)
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
        qDebug() << "Make_Merge_File::tackle_solo_file_get_all";
        throw;
    }
}

void Make_Merge_File::profile_target_data_params(vector<vector<string>> &all_array,
                                                 size_t &rows_num,
                                                 size_t &cols_num,
                                                 size_t &target_data_index)
{
    try {
        // 记录数据有效的标记
        bool valid_row_flage = false;
        // 遍历分析目标数据的行列数
        // 判断是否找到了目标点
        bool target_index_flage = false;
        for(size_t i = 0; i < all_array.size(); i++)
        {
            // 跳过无效的空行
            if(all_array[i].size() == 0 || all_array[i][0].size() == 0)continue;
            // 找到目标数据的起点
            // 非理想情况，则判断后跳回重新判断理想位置【必须位于target_str判断你的前方，进行i值和数据的修正】
            if(all_array[i][0] == "Unit" && !target_index_flage) // "Unit"点
            {
                // 进来这里，说明前面没找到目标点，则对目标点做重新修改后，重新判断
                i -= 1;
                all_array[i][0] = target_str;
            }
            else if(all_array[i][0] == "LimitL" && !target_index_flage) // "LimitL"点
            {
                // 进来这里，说明前面没找到目标点，则对目标点做重新修改后，重新判断
                i -= 2;
                all_array[i][0] = target_str;
            }
            else if(all_array[i][0] == "LimitU" && !target_index_flage) // "LimitU"点
            {
                // 进来这里，说明前面没找到目标点，则对目标点做重新修改后，重新判断
                i -= 3;
                all_array[i][0] = target_str;
            }

            // 理想情况，找到目标的起点位置
            if(all_array[i][0] == target_str)
            {
                // 如果找到目标，记录开始位置
                target_data_index = i;
                // 目标位置的字符串个数，即cols_num的有效列数
                cols_num = all_array[i].size();
                // 设置从此刻开始，记录有效行的个数
                valid_row_flage = true;
                target_index_flage = true;
            }

            // 如果满足flage，则rows_num++
            if(valid_row_flage)
            {
                rows_num++;
            }
        }
    } catch (...) {
        qDebug() << "Make_Merge_File::profile_target_data_params";
        throw;
    }
}

vector<vector<string>> Make_Merge_File::tackle_solo_file_get_target(vector<vector<string>> &all_array,
                                                                     size_t rows_num,
                                                                     size_t cols_num,
                                                                     size_t target_data_index)
{
    try {
        vector<vector<string>> target_arrays;

        // 经过profile_col_row_num优化后的目标获取的循环
        // 获取目标数据
        for(size_t i = target_data_index;i < all_array.size();i++)
        {
            // 输入的数据需要补0，长度不够要求
            while(all_array[i].size() != cols_num) all_array[i].push_back(""); // 补0
            target_arrays.push_back(all_array[i]); // 将结果直接插入
        }

        // test_datas(target_arrays);

        return target_arrays;

    } catch (...) {
        qDebug() << "Make_Merge_File::tackle_solo_file_get_target";
        throw;
    }
}

void Make_Merge_File::profile_div_index(const vector<vector<string>> &target_datas,
                                        size_t &end_head_begin_body)
{
    try {
        size_t& i = end_head_begin_body;
        for(i = 0;i < target_datas.size();i++)
        {
            if(target_datas[i][0] == this->div_str || target_datas[i].size() == 0) break;
        }

        // 空行的下一位为数据起始位，保证结尾为开区间'[)'
        i++;

    } catch (...) {
        qDebug() << "Make_Merge_File::profile_div_index";
        throw;
    }
}

vector<vector<string>> Make_Merge_File::get_head_datas(const vector<vector<string>> &target_datas,
                                                       size_t end_head_begin_body)
{
    try {
        vector<vector<string>> head_datas;

        for(size_t i = 0;i < end_head_begin_body;i++)
        {
            head_datas.push_back(target_datas[i]);
        }

        return head_datas;

    } catch (...) {
        qDebug() << "Make_Merge_File::get_head_datas";
        throw;
    }

}

vector<vector<string>> Make_Merge_File::get_body_datas(const vector<vector<string>> &target_datas,
                                                       size_t end_head_begin_body)
{
    try {
        vector<vector<string>> body_datas;

        for(size_t i = end_head_begin_body;i < target_datas.size();i++)
        {
            body_datas.push_back(target_datas[i]);
        }

        return body_datas;

    } catch (...) {
        qDebug() << "Make_Merge_File::get_body_datas";
        throw;
    }
}

vector<vector<string>> Make_Merge_File::update_body_datas(const vector<vector<string>> &body_datas)
{
    try {
        vector<vector<string>> update_body;

        for(size_t i = 0;i < body_datas.size();i++)
        {
            update_body.push_back(body_datas[i]);

            // 修改PART_ID【一个file使用同一个counter】
            update_body[i][1] = to_string(counter);

        }
        counter++;

        return update_body;

    } catch (...) {
        qDebug() << "Make_Merge_File::update_body_datas";
        throw;
    }
}

void Make_Merge_File::load_datas(const vector<vector<string>> &datas)
{
    try {
        for(size_t i = 0;i < datas.size();i++)
        {
            merge_datas.push_back(datas[i]);
        }

    } catch (...) {
        qDebug() << "Make_Merge_File::load_datas";
        throw;
    }
}

void Make_Merge_File::profile_output_file_path() noexcept
{
    try {
        merge_file_path = QCoreApplication::applicationDirPath() +
                "/" + merge_file_name;

    } catch (...) {
        qDebug() << "Make_Merge_File::profile_output_file_path";
        throw;
    }
}

void Make_Merge_File::save_datas()
{
    try {
        // 获取文件输出流
        ofstream ofs = output_file_open(merge_file_path.toStdString());

        // 将温度数据内容写出
        for(size_t i = 0;i < merge_datas.size(); i++)
        {
            for(size_t j = 0;j < merge_datas[i].size();j++)
            {
                ofs<<merge_datas[i][j]<<",";
            }
            ofs<<endl;
        }

        ofs.close();
    } catch (...) {
        qDebug() << "Make_Merge_File::save_datas";
        throw;
    }
}

void Make_Merge_File::test_datas(const vector<vector<string>> &arrays)
{
    try {
        for(size_t i = 0;i < arrays.size(); i++)
        {
            for(size_t j = 0;j < arrays[i].size();j++)
            {
                cout<<arrays[i][j]<<" ";
            }
            cout<<endl;
        }

    } catch (...) {
        qDebug() << "Make_Merge_File::test_datas";
        throw;
    }
}
