#ifndef TARGETFILE_TEMPERATURE_VALID_DATA_H
#define TARGETFILE_TEMPERATURE_VALID_DATA_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Attri_Unit_Ul.h"
#include "Scatter_Site_X_Part.h"
using namespace std;

class Targetfile_Temperature_Valid_Data
{
public:
    Targetfile_Temperature_Valid_Data();
    ~Targetfile_Temperature_Valid_Data();

    // 功能四：温度数据处理
    void total_task(const string target_file_path);

    // 【get区】
    // 根据属性获取单位
    string get_attri_unit(const string& attri) noexcept;
    // 获取属性本身定义的最大最小值
    pair<double,double> get_attri_define_XI(const string& attri) noexcept;
    // 获取属性的数值的最大最小值
    pair<double,double> get_attri_value_XI(const string& attri) noexcept;

    // 获取scatter_site
    vector<string> get_scatter_site();
    // 获取scatter_part
    vector<int> get_scatter_part();

    // 获取temperature目标文件读入程序后的完整数据
    vector<vector<string>> get_temperature_target_file_vec() noexcept;
    // 获取生成线数据所需的结构体，所有坐标类型都采用离散方式实现
    map<string,map<string,map<int,double>>> get_series_datas() noexcept;

    // 获取属性标签
    const vector<string> get_labels() noexcept;

private:
    // 【方法区】
    // 获得文件输入流
    ifstream input_file_open(const string& input_file_path);

    // 将temperature_target_file内容载入程序中
    void load_target_file(const ifstream& ifs);
    // 分析头数据，体数据分界点
    void profile_head_body_div();
    // 分析数据有效列
    void profile_valid_col();
    // 分析获取其中的有效数据
    void profile_valid_datas();
    // 分析获取所有属性
    void profile_labels();

    // 具体分析的有效数据
    void profile_site_part();
    void profile_uul();
    void profile_series_data();

    // 【获取程序中需要的参数数据】
    // 判断字符串是否为整数【site不一定为整数，part_id一定为整数】
    bool is_Integer_Numeric(const string &str);
    // 根据给定的属性，获取其在all_array中所在的行索引
    size_t get_row_index_by_attri(const string& attri);



    // 【对象区】
    // 获取temperature_target_file的所有内容
    vector<vector<string>> m_all_array;
    // 拆分获取的有效数据
    Scatter_Site_X_Part m_site_part;
    Attri_Unit_Ul m_uul;
    map<string,map<string,map<int,double>>> m_series_data;



    // 【有效数据的默认属性区】
    // target_file的数据分界点
    const string div_attri = "TEST_NUM";
    // 头数据的结束点，体数据的起始点
    size_t end_head_begin_body = 0;
    // 有效数据的起始列
    size_t valid_col = 0;

    // 所有属性
    vector<string> labels;
};

#endif // TARGETFILE_TEMPERATURE_VALID_DATA_H
