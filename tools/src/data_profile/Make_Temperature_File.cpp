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

const string Make_Temperature_File::total_task(const QStringList &file_paths)
{
    /*
        功能：
            遍历所有获取的temperature文件路径，处理这些文件，并将结果存储在temperature_datas中，
            最后将内容写入温度文件中
    */
    try {
        // 遍历所有温度文件
        for(int i = 0; i < file_paths.size();i++)
        {
            // 送入单文件处理，进行处理
            tackle_solo_file(file_paths[i]);
        }

        // 处理所有文件结束，将文件内容进行保存
        save_datas();

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
        string last_no;
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

void Make_Temperature_File::tackle_solo_file(const QString &file_path)
{
    try {


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

    } catch (...) {
        qDebug() << "Make_Temperature_File::profile_target_data_params";
        throw;
    }
}

vector<vector<string>> Make_Temperature_File::tackle_solo_file_get_target(const vector<vector<string> > &all_array)
{
    try {

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

vector<vector<string>> Make_Temperature_File::update_body_datas(const vector<vector<string>> &body_datas,
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

        for(size_t i = 0;i < body_datas.size();i++)
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

    } catch (...) {
        qDebug() << "Make_Temperature_File::save_datas";
        throw;
    }
}

void Make_Temperature_File::test_datas(const vector<vector<string> > &arrays)
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
