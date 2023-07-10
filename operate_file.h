#ifndef HOPERATE_FILE_H
#define HOPERATE_FILE_H

#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>
#include<sstream>
using namespace std;
#define endl '\n'

// 需要测试的属性名称
const vector<string> labels = {"SITE_NUM","PART_ID","Continuity_out","Continuity_vin1","Continuity_en","Continuity_pg",
"Continuity_pgdl","Continuity_fb","Leakage_out_l","Leakage_en_l","Leakage_pg_l","Leakage_pgdl_l","Leakage_fb_l",
"Ignd100","Ishd","Vfb","Vout_5","Ifb","Ilimit","Vdropout_5_50mA","Vdropout_5_300mA","Vline_regulation_5",
"Vload_regulation_33","Tss","Ven_low","Ven_high","Ien","PG_rising","PG_hys","PG_low","PG_leakage",
"PGDL_charg","PGDL_rising","PGDL_falling","PG_reaction"};

// 处理的文件名
const string IN_FILE_NAME = "D:\\QT\\Codes\\profile_Excel_By_Qt\\source_file.csv";
const string OUT_FILE_NAME = "D:\\QT\\Codes\\profile_Excel_By_Qt\\target_file.csv";

const string task01();
ifstream input_file_open(const string& open_File_Name);
ofstream output_file_open(const string& open_File_Name);
vector<vector<string>> tackle_file(const ifstream& ifs);
vector<vector<string>> tackle_file_all(const ifstream& ifs);
vector<vector<string>> tackle_file_get_target(const vector<vector<string>>& all_array);
vector<vector<string>> tackle_file_rows(const vector<vector<string>>& target_arrays);
vector<vector<string>> tackle_file_cols(const vector<vector<string>>& target_arrays);
// map<string,vector<string>> tackle_file_get_maps(const vector<vector<string>>& cols_array);
vector<vector<string>> tackle_file_get_ans(const vector<vector<string>>& rows_array,const vector<vector<string>>& cols_array);
void save_tackle_datas(const ofstream& ofs,const vector<vector<string>>& datas);

// 暂时没什么用处
void draw_datas(const string& draw_File_Name);

// test
void test_datas(const vector<vector<string>>& arrays);

#endif
