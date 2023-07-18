#ifndef FILE_TO_TARGETFILE_H
#define FILE_TO_TARGETFILE_H

#include<iostream>
#include<fstream>
#include<vector>
#include <QString>
#include <QDebug>
using namespace std;
//#define endl '\n'

class File_To_Targetfile{
public:
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
    const string total_task();

private:
    // 动态获取输入文件的绝对路径
    QString profile_input_file_path();

    // 动态获取输出文件的绝对路径
    QString profile_output_file_path(QString output_file_name = "target_file.csv");

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

    // 处理输入的源文件，获取目标数据后，读入内存中
    vector<vector<string>>
    tackle_file(const ifstream& ifs);
    vector<vector<string>>
    tackle_file_get_all(const ifstream& ifs);
    // 获取并更新目标数据的长和宽更新数据【方便嵌套已有程序】
    void profile_col_row_num(const vector<vector<string>>& all_array);
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



// 需要测试的属性名称【不需要指定属性，所有属性都进行显示即可】
//const vector<string> labels = {"SITE_NUM","PART_ID","Continuity_out","Continuity_vin1","Continuity_en","Continuity_pg",
//"Continuity_pgdl","Continuity_fb","Leakage_out_l","Leakage_en_l","Leakage_pg_l","Leakage_pgdl_l","Leakage_fb_l",
//"Ignd100","Ishd","Vfb","Vout_5","Ifb","Ilimit","Vdropout_5_50mA","Vdropout_5_300mA","Vline_regulation_5",
//"Vload_regulation_33","Tss","Ven_low","Ven_high","Ien","PG_rising","PG_hys","PG_low","PG_leakage",
//"PGDL_charg","PGDL_rising","PGDL_falling","PG_reaction"};

// 可修改的输入文件的路径【通过函数接口可以进行修改】
string IN_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\source_file.csv";
// 内部可修改输出文件的产生位置
string OUT_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\target_file.csv";

// 目标表的长和宽【以SITE_NUM作为目标数据的开始标记，更新目标数据的长和宽】
size_t cols_num = 45;
size_t rows_num = 45;
size_t targe_data_index = 0; // 记录有效数据开始的起点
};

#endif // FILE_TO_TARGETFILE_H
