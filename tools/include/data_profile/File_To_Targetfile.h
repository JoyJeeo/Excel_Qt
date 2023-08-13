#ifndef FILE_TO_TARGETFILE_H
#define FILE_TO_TARGETFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDebug>
using namespace std;
//#define endl '\n'

class File_To_Targetfile{
public:
    File_To_Targetfile();
    // 打开输入和输出文件
    ifstream input_file_open(const string& input_File_path);
    ofstream output_file_open(const string& out_File_path);

    // SET,GET区域
    // 修改输入文件的路径
    void set_input_file_path(const string& path) noexcept;
    // 查看输入文件的路径
    string get_input_file_path() noexcept;

    // 直接实现该类的总任务
    // 将target_file文件的路径进行返回
    const string total_task(const string& input_file_path,string output_file_name = "target_file.csv");

    // 单数据多文件合并
    const string merge_task(const QStringList& file_paths);

    // 不同时刻的同一个No比较
    const string time_task(const QStringList& dir_paths);
    // 生成ration的task【耦合】 // 这个任务必须在time_task任务执行完成，使用完target文件后，才应该可以被调用
    const string ration_task();

    // 功能四
    const string temperature_task(const QStringList& file_paths);

private:
    // 动态获取输入文件的绝对路径
    QString profile_input_file_path();
    // 动态获取输出文件的绝对路径
    QString profile_output_file_path(string output_file_name = "target_file.csv");


    // 将输入的QString -> string
    /*
        字符串类型转换：
            QString->string: QString("123").toStdString()
            string->char*: string("123").c_str()
            char*->string: string("123")
            string->QString: QString::fromStdString(string("123"))

            char*->QString: QObject::tr("123")
    */
    string qstring_to_string(const QString& qstr);

    // SET,GET区域
    // 修改输出文件的路径
    void set_output_file_path(const string& path) noexcept;
    // 查看输出文件的路径
    string get_output_file_path() noexcept;
    // 修改alls文件的路径
    void set_alls_file_path(const string& path) noexcept;
    // 查看alls文件的路径
    string get_alls_file_path() noexcept;
    // 修改timc文件的路径
    void set_timc_file_path(const string& path) noexcept;
    // 查看timc文件的路径
    string get_timc_file_path() noexcept;
    // 修改ration文件的路径
    void set_ration_file_path(const string& path) noexcept;
    // 查看ration文件的路径
    string get_ration_file_path() noexcept;

    // 处理输入的源文件，获取目标数据后，读入内存中
    vector<vector<string>>
    tackle_file(const ifstream& ifs);
    vector<vector<string>>
    tackle_file_get_all(const ifstream& ifs);
    // 获取并更新目标数据的长和宽更新数据【方便嵌套已有程序】 // 【只用这一个分析，因为他们三个之间互相是有关系的】
    // 由于数据可能存在编码问题，可能需要修改all_array中的部分数据为目标数据
    void profile_col_row_num(vector<vector<string>>& all_array);
    vector<vector<string>>
    tackle_file_get_target(const vector<vector<string>>& all_array);
    vector<vector<string>>
    tackle_file_get_rows(const vector<vector<string>>& target_arrays);
    vector<vector<string>>
    tackle_file_get_cols(const vector<vector<string>>& target_arrays);
    vector<vector<string>>
    tackle_file_get_ans(const vector<vector<string>>& rows_array,const vector<vector<string>>& cols_array); 

    // 将读入程序中的数据输出到文件中
    void save_tackle_datas(const ofstream& ofs,const vector<vector<string>>& datas);

    // string之间，忽略大小写的比较【由于读取字符时，莫名会从大写转为小写，因此进行处理】
    bool str_cmp_ignore_case(const string& a,const string& b) noexcept;

    // 测试载入程序的数据是否符合预期
    void test_datas(const vector<vector<string>>& arrays);

    // 判断一个string是否为整数数字
    bool is_Integer_Numeric(const string &str);
    // 获取属性在m_source_target_file_vec中所对应的有效列索引值【专门给cols_array中的数据使用】
    size_t get_vec_col_index_valid(const vector<vector<string>>& target_arrays);

    // 【功能二】
    // 多文件单数据合并功能【依然返回target_file的文件路径】
    void merge_files_solo_data (const QStringList& file_paths);
    // 将输入数据的part_id进行替换
    void update_data_part(string& data,int part);
    // 查找某个符号第几次出现再string中的位置
    int find_str_tag_dex(const string& data,const char& c,int count);

    // 【功能三】
    void profile_pro_path() noexcept;
    // 创建存储时刻合并的文件夹
    const QString build_time_dir();
    // 清空时刻文件夹中的所有内容
    void clear_time_dir(const QString& time_dir);
    void tackle_single_dir(QString dir_path,vector<vector<string>>& time_datas,bool& flage);
    // 将同一时刻下的文件进行合并
    void time_file_merge(const QStringList& file_paths,vector<vector<string>>& time_datas,
                         bool& flage,QString time_name);
    // 分析时刻文件夹的时刻名称
    QString profile_time_file_name(const string& dir_path);


// 项目所在的文件路径
QString pro_path = ".";
// 可修改的输入文件的路径【通过函数接口可以进行修改】
string IN_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\source_file.csv";
// 内部可修改输出文件的产生位置
string OUT_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\target_file.csv";
// 合并的输出文件的产生位置
string ALLS_MERGE_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\alls.csv";
// 时刻文件的合并文件
string TIME_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\timc.csv";
// 变动率数据文件的文件位置
string RATION_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\ration.csv";

// 中间文件存储的文件夹名称
const QString time_file_name = "timc.csv";
const QString ration_file_name = "ration.csv";

// 有效数据开始的起始位置
const string target_str = "SITE_NUM";


// 目标表的长和宽【以SITE_NUM作为目标数据的开始标记，更新目标数据的长和宽】
size_t cols_num = 45;
size_t rows_num = 45;
size_t targe_data_index = 0; // 记录有效数据开始的起点
};


#endif // FILE_TO_TARGETFILE_H
