#include "tools/include/data_profile/Make_Temperature_File.h"

#include <QDebug>
#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <sstream>

Make_Temperature_File::Make_Temperature_File()
{
    try {
        profile_output_file_path();

    } catch (...) {
        qDebug() << "Make_Temperature_File::Make_Temperature_File";
        throw;
    }
}

Make_Temperature_File::~Make_Temperature_File()
{

}

const string Make_Temperature_File::make_temperature_file(const QStringList &file_paths)
{
    /*
        功能：
            遍历所有获取的temperature文件路径，处理这些文件，并将结果存储在temperature_datas中，
            最后将内容写入温度文件中
    */
    try {
        // 只有第一个文件才会读取头数据
        bool first_file_flage = true;

        // 遍历所有温度文件
        for(int i = 0; i < file_paths.size();i++)
        {
            // 送入单文件处理，进行处理
            tackle_solo_file(file_paths[i],first_file_flage);

            // 拒绝获取头部数据
            if(i == 0) first_file_flage = false;
        }

        // 处理所有文件结束，将文件内容进行保存
        save_datas();

        // 判断是否需要展示warning
        if(test_plan.warning_flage())
            test_plan.warning_show();
        if(test_plan.warning_extra_flage())
            test_plan.warning_extra_show();

        // 返回保存的temperature文件的路径
        return temperature_file_path.toStdString();

    } catch (...) {
        qDebug() << "Make_Temperature_File::total_task";
        throw;
    }
}

ifstream Make_Temperature_File::input_file_open(const string &input_file_path)
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
        qDebug() << "Make_Temperature_File::input_file_open";
        throw;
    }
}

ofstream Make_Temperature_File::output_file_open(const string &out_file_path)
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
        qDebug() << "Make_Temperature_File::output_file_open";
        throw;
    }
}

pair<vector<string>,string> Make_Temperature_File::profile_siteNo_temperature(const string &file_name)
{
    /*
        功能：
            将文件名中的N01,N02 -> No_01,No_02
            将温度中的m40 -> -40，其余直接存入即可【string类型的int】

        返回值：
            first：No_01,...的容器
            second：string类型的int数值

        说明：
            文件名以N01-N02-N03_125.csv的格式命名
            一般只会有m40,0,25,85,125的℃温度
    */
    try {
        // 将文件名作为输入端重新读入
        istringstream sin(file_name);

        // 存储字符串段
        vector<string> str_names;
        string temp_str;

        // 将file_name以'_'为分界点，进行字符串内容读取
        while(getline(sin,temp_str,'-'))
        {
            // cout<<array.size()<<" ";
            str_names.push_back(temp_str);
        }

        // 【对字符串段进行分析】
        // 对分割的字符串进行分析
        int len = str_names.size();
        int last_index = len - 1;

        // 存储No的容器
        vector<string> nos;
        // 存储数值的字符串
        string temperture = "";

        // 分析no
        for(int i = 0;i < last_index;i++)
        {
            string no = str_names[i];
            string ans_no = "No_" + no.substr(1,no.size() - 1);


            // 加入nos
            nos.push_back(ans_no);
        }

        // 分析末尾的特殊字符串【N03_125.csv】
        string last_str = str_names[last_index];
        string last_no = "";
        bool tmp_flage = false; // 用于两种数据获取的切换
        for(size_t i = 0;i < last_str.size();i++)
        {
            // no处理
            if(!tmp_flage)
            {
                if(last_str[i] == '_')
                {
                    // 做最后的处理
                    nos.push_back(last_no);

                    tmp_flage = true; // 循环内容的切换
                    continue;
                }

                // 获取no
                if(last_str[i] != 'N')
                {
                    last_no += last_str[i];
                }
                // 遇到'N'
                else {
                    last_no = "No_";
                }

            }
            // 温度值处理
            else {
                if(last_str[i] == '.') break;

                // 获取温度值
                if(last_str[i] == 'm' || last_str[i] == 'M')
                {
                    temperture += '-';
                }
                else {
                    temperture += last_str[i];
                }
            }
        }


        return make_pair(nos,temperture);


    } catch (...) {
        qDebug() << "Make_Temperature_File::profile_siteNo_temperature";
        throw;
    }
}

void Make_Temperature_File::tackle_solo_file(const QString &file_path,bool first_file_flage)
{
    try {
        // 获取文件路径中的文件名
        string file_name = profile_solo_file_name(file_path);

        // 分析文件名获取解析结果的pair
        pair<vector<string>,string> steers = profile_siteNo_temperature(file_name);

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
        vector<vector<string>> target_array = tackle_solo_file_get_target(all_array,rows_num,cols_num,target_data_index);

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
        vector<vector<string>> update_body = update_body_datas(body_datas,steers);
        // 加载进容器中
        load_datas(update_body);


    } catch (...) {
        qDebug() << "Make_Temperature_File::tackle_solo_file";
        throw;
    }
}

string Make_Temperature_File::profile_solo_file_name(const QString &file_path)
{
    try {
        // 将QString转换为string进行运算
        string tmp_file_path = file_path.toStdString();

        size_t i;

        for(i = tmp_file_path.size() - 1;i >=0 ;i--)
        {
            if(tmp_file_path[i] == '/' || tmp_file_path[i] == '\\')break;
        }
        i++;
        // 设置获取的文件名
        string file_name = tmp_file_path.substr(i,tmp_file_path.size() - i); // 由于0会补+1，因此直接-i即可

        return file_name;

    } catch (...) {
        qDebug() << "Make_Temperature_File::profile_solo_file_name";
        throw;
    }
}

vector<vector<string>> Make_Temperature_File::tackle_solo_file_get_all(const ifstream &ifs)
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
        qDebug() << "Make_Temperature_File::tackle_solo_file_get_all";
        throw;
    }
}

void Make_Temperature_File::profile_target_data_params(vector<vector<string> > &all_array,
                                                       size_t &rows_num, size_t &cols_num,
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
        qDebug() << "Make_Temperature_File::profile_target_data_params";
        throw;
    }
}

vector<vector<string>> Make_Temperature_File::tackle_solo_file_get_target(vector<vector<string> > &all_array,
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
        qDebug() << "Make_Temperature_File::tackle_solo_file_get_target";
        throw;
    }
}

void Make_Temperature_File::profile_div_index(const vector<vector<string> > &target_datas,
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
        qDebug() << "Make_Temperature_File::profile_div_index";
        throw;
    }
}

vector<vector<string>> Make_Temperature_File::get_head_datas(const vector<vector<string>> &target_datas,
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
        qDebug() << "Make_Temperature_File::get_head_datas";
        throw;
    }
}

vector<vector<string>> Make_Temperature_File::get_body_datas(const vector<vector<string>> &target_datas,
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
        qDebug() << "Make_Temperature_File::get_body_datas";
        throw;
    }
}

vector<vector<string>> Make_Temperature_File::update_body_datas(
                                    const vector<vector<string>> &body_datas,
                                    const pair<vector<string>,string> &steers)
{
    /*
        功能：
            将body中的site换为No，partid换位对应的温度数值【string的int数值】

        说明：
            默认在测试机台上使用site时，是按照
                1. site从小到达进行使用【对应温度文件中SITE_NUM从小到大】
                2. site上的No芯片编号，也是按照从小到大进行排序
                    【对应site上的No是从小到大，且文件名中No也是从小到大】
            总结：site使用，从小到大；site上No放取，从小到大；生成的数据文件中No编号书写，从小到大
                【三从小到大】
    */
    try {
        vector<vector<string>> update_body;

        // 为了防止测试机给出多余的数据造成越界问题，这里循环使用steers.first明确指明的site数，这样多余的数据，在程序内就会被删除掉
        for(size_t i = 0;i < steers.first.size();i++)
        {
            update_body.push_back(body_datas[i]);
            // 修改SITE_NUM
            // 一般将文件名中的芯片编号N01...，改为No_01,No_02,...
            update_body[i][0] = steers.first[i];

            // 修改PART_ID
            update_body[i][1] = steers.second;

        }

        return update_body;

    } catch (...) {
        qDebug() << "Make_Temperature_File::update_body_datas";
        throw;
    }
}

void Make_Temperature_File::load_datas(const vector<vector<string>> &datas)
{
    /*
        功能：
            将传入的datas，加入温度数据容器中

        此函数的意义：
            无论是任何数据，只要符合vector<vector<string>>数据类型，那么就可以将这个数据内容加载进温度容器，
            【这样的加载方法，很大程度上提高了代码的复用性和程序可读性】
    */
    try {
        for(size_t i = 0;i < datas.size();i++)
        {
            temperature_datas.push_back(datas[i]);
        }

    } catch (...) {
        qDebug() << "Make_Temperature_File::load_datas";
        throw;
    }
}

void Make_Temperature_File::profile_output_file_path() noexcept
{
    temperature_file_path = QCoreApplication::applicationDirPath() +
            "/" + temperature_file_name;
}

void Make_Temperature_File::save_datas()
{
    try {
        // 获取文件输出流
        ofstream ofs = output_file_open(temperature_file_path.toStdString());

        // 将温度数据内容写出
        for(size_t i = 0;i < temperature_datas.size(); i++)
        {
            for(size_t j = 0;j < temperature_datas[i].size();j++)
            {
                ofs<<temperature_datas[i][j]<<",";
            }
            ofs<<endl;
        }

        ofs.close();

    } catch (...) {
        qDebug() << "Make_Temperature_File::save_datas";
        throw;
    }
}

void Make_Temperature_File::test_datas(const vector<vector<string>> &arrays)
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
        qDebug() << "Make_Temperature_File::test_datas";
        throw   ;
    }
}
