#ifndef TARGETFILE_VALID_DATA_H
#define TARGETFILE_VALID_DATA_H

#include "Site_Part.h"
#include "Attri_Unit_Ul.h"
#include <QDebug>

// NULL的取固定值
const int NULL_Number = INT_MIN;


// 获取数据表中所有数据拆分后形成的有效数据的类【直接使用类来对目标数据进行封装的方法，很好的实现数据的处理和代码的逻辑实现】
class Targetfile_Valid_Data // 数据表中有效数据的拆分结果
{
public:

    // 【以下函数直接对类成员进行"修改"】
    // 【命名方式中使用m_ 或者 get_等，目的在于方便对象和目标函数的智能搜索获取】

    // 获取target_file主数据区的属性数据的最大值和最小值
    // 根据获取的属性attri，来获取其对应属性数据中的最大和最小值
    pair<double,double> get_attri_XI(const string& attri);

    // 获取get_attri_max_min函数返回数据与LimitL和LimitU对比返回的最大值和最小值
    pair<double,double> get_ul_compare_attri_XI(const string& attri);


    // 通过函数接口获得成员对象
    /* 这里成员对象获取的逻辑是:
     *  1. 通过load_target_vec构造获得m_source_target_file_vec
     *  2. 通过profile_*函数构造其他成员【成员构造的逻辑】
     *  3. 通过get_*接口获得所有成员，以对外使用
    */
    vector<vector<string>> get_source_target_file_vec() noexcept;

    Site_Part get_site_parts() noexcept;

    Attri_Unit_Ul get_attri_uuls() noexcept;

    map<string,map<int,vector<double>>> get_series_datas() noexcept;

    const vector<string> get_labels() noexcept;

    // 【功能三】
    map<string,map<string,vector<double>>> get_time_series_datas() noexcept;
    const vector<string> get_time_labels() noexcept;
    Site_Part get_time_site_parts() noexcept;
    Attri_Unit_Ul get_time_attri_uuls() noexcept;
    vector<vector<string>> get_timc_target_file_vec() noexcept;
    pair<double,double> get_time_attri_XI(const string& attri);
    pair<double,double> get_time_ul_compare_attri_XI(const string& attri);



    // total_task
    bool total_task(const ifstream& ifs);

    // 【功能三】
    bool time_task(const ifstream& ifs);

private:
    // 获取site和part
    void profile_get_site_parts();
    // 获取unit,limitL和limitU
    void profile_get_attri_uuls();
    // 获取线的数据组，实际就是获取map<string,vector<vector<double>>>
    void profile_get_series_datas();
    // 获取数据的vector<vector<double>>>的map数据【将string转换为double存储在矩阵中】
    // 将源数据，进行解析拆分，是对三个成员函数的统一分析处理
    void profile_get_datas();
    // 获取target_file表的所有结果【加载数据】【实现函数封装，不使用之前写好的函数，这样破坏了封装性】【实现函数封装】
    void load_target_vec(const ifstream& ifs);

    // 判断一个string是否为整数数字
    bool is_Integer_Numeric(const string &str);
    // 获取属性在m_source_target_file_vec中所对应的行索引值
    size_t get_target_vec_row_index_by_attri(const string& attri);
    // 获取属性在m_source_target_file_vec中所对应的有效列索引值
    size_t get_target_vec_col_index_valid();
    // 动态分析表格中的属性，维护labels
    void profile_labels();

    // test
    void test_for(map<int,vector<double>>& datas);
    void test_for(map<string,vector<double>>& datas);

    // 全读取target_file中的数据
    vector<vector<string>> m_target_file_vec;

    // 使用组合的方式，使用其他类的对象，这样便于数据封装与方法调用和实现
    // 获取的主要Valid_datas
    Site_Part m_site_parts;
    Attri_Unit_Ul m_attri_uuls; // attri -> uul 他们之间具有直接的关系
//    map<string,vector<vector<double>>> m_series_datas; // 因为attri和site都属于离散的数据，并不是连续的像part那样，因此必须解耦
    map<string,map<int,vector<double>>> m_series_datas; // attri -> [ site -> [ val | index:part ] ] // attri,site都为唯一数据

    // 【功能三】
    map<string,map<string,vector<double>>> m_time_series_datas;
    void profile_get_time_datas();
    // 获取site和part
    void profile_get_time_site_parts();
    // 获取unit,limitL和limitU
    void profile_get_time_attri_uuls();
    // 获取线的数据组，实际就是获取map<string,vector<vector<double>>>
    void profile_get_time_series_datas();
    // 动态分析表格中的属性，维护labels
    void profile_time_labels();

    // 用于区分属性的分界点属性【数据表自身的"属性"】
    const string div_attri = "TEST_NUM";

    // 使用profile_labels函数，更新不同的csv文件中的目标属性 // 规定map的打印顺序所需的key数据
    vector<string> labels;
};


#endif // VALID_DATA_H
