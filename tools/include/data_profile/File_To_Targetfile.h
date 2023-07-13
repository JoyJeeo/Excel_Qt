#ifndef FILE_TO_TARGETFILE_H
#define FILE_TO_TARGETFILE_H

#include<iostream>
#include<fstream>
#include<vector>
#include <QString>
using namespace std;
#define endl '\n'

class File_To_Targetfile{
public:
    // SET,GET区域
    // 修改输入文件的路径
    void set_input_file_path(const string& path);
    // 查看输入文件的路径
    string get_input_file_path();

    // 直接实现该类的总任务
    // 将target_file文件的路径进行返回
    const string total_task();

private:
    // 打开输入和输出文件
    ifstream input_file_open(const string& input_File_path);
    ofstream output_file_open(const string& out_File_path);

    // 处理输入的源文件，获取目标数据后，读入内存中
    vector<vector<string>>
    tackle_file(const ifstream& ifs);
    vector<vector<string>>
    tackle_file_get_all(const ifstream& ifs);
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

    // SET,GET区域
    // 修改输出文件的路径
    void set_output_file_path(const string& path);
    // 查看输出文件的路径
    string get_output_file_path();

    // 测试载入程序的数据是否符合预期
    void test_datas(const vector<vector<string>>& arrays);

    // 获取输入文件的绝对路径
    QString profile_input_file_path();

    // 获取输出文件的绝对路径
    QString profile_output_file_path(QString output_file_name);

    // 将输入的QString -> string
    string qstring_to_string(const QString& qstr);



// 需要测试的属性名称
const vector<string> labels = {"SITE_NUM","PART_ID","Continuity_out","Continuity_vin1","Continuity_en","Continuity_pg",
"Continuity_pgdl","Continuity_fb","Leakage_out_l","Leakage_en_l","Leakage_pg_l","Leakage_pgdl_l","Leakage_fb_l",
"Ignd100","Ishd","Vfb","Vout_5","Ifb","Ilimit","Vdropout_5_50mA","Vdropout_5_300mA","Vline_regulation_5",
"Vload_regulation_33","Tss","Ven_low","Ven_high","Ien","PG_rising","PG_hys","PG_low","PG_leakage",
"PGDL_charg","PGDL_rising","PGDL_falling","PG_reaction"};

// 可修改的输入文件的路径【通过函数接口可以进行修改】
string IN_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\source_file.csv";
// 限制输出文件的产生位置
string OUT_FILE_PATH = "D:\\QT\\Codes\\profile_Excel_By_Qt\\target_file.csv";
};

#endif // FILE_TO_TARGETFILE_H
