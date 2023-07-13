#ifndef VALID_DATA_H
#define VALID_DATA_H

#include "Site_Part.h"
#include "Attri_Unit_Ul.h"


// NULL的取固定值
const int NULL_Number = INT_MIN;


// 获取数据表中所有数据拆分后形成的有效数据的类【直接使用类来对目标数据进行封装的方法，很好的实现数据的处理和代码的逻辑实现】
class Valid_Data // 数据表中有效数据的拆分结果
{
public:

    // 【以下函数直接对类成员进行"修改"】
    // 【命名方式中使用m_ 或者 get_等，目的在于方便对象和目标函数的智能搜索获取】
    // 获取site和part
    void profile_get_site_parts();

    // 获取unit,limitL和limitU
    void profile_get_attri_uuls(const string& normal_attri);

    // 获取线的数据组，实际就是获取map<string,vector<vector<double>>>
    void profile_get_series_datas(const string& normal_attri);

    // 获取数据的vector<vector<double>>>的map数据【将string转换为double存储在矩阵中】
    // 将源数据，进行解析拆分，是对三个成员函数的统一分析处理
    void profile_get_datas();

    // 获取target_file表的所有结果【加载数据】【实现函数封装，不使用之前写好的函数，这样破坏了封装性】【实现函数封装】
    void load_target_vec(const ifstream& ifs);

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
    vector<vector<string>> get_source_target_file_vec();

    Site_Part get_site_parts();

    Attri_Unit_Ul get_attri_uuls();

    map<string,vector<vector<double>>> get_series_datas();

    const vector<string> get_labels();

private:
    // 判断一个string是否为整数数字
    bool is_Integer_Numeric(const string &str);
    // 获取属性在m_source_target_file_vec中所对应的行索引值
    size_t get_source_vec_row_index_by_attri(const string& attri);
    // 获取属性在m_source_target_file_vec中所对应的有效列索引值
    size_t get_source_vec_col_index_valid();

// 获取主数据
vector<vector<string>> m_source_target_file_vec; // target_file中直接获取的数据
// 使用组合的方式，使用其他类的对象，这样便于数据封装与方法调用和实现
Site_Part m_site_parts;
Attri_Unit_Ul m_attri_uuls; // attri -> uul 他们之间具有直接的关系
map<string,vector<vector<double>>> m_series_datas;
// 规定map的打印顺序所需的key数据
const vector<string> labels = {"SITE_NUM","PART_ID","Continuity_out","Continuity_vin1","Continuity_en","Continuity_pg",
"Continuity_pgdl","Continuity_fb","Leakage_out_l","Leakage_en_l","Leakage_pg_l","Leakage_pgdl_l","Leakage_fb_l",
"Ignd100","Ishd","Vfb","Vout_5","Ifb","Ilimit","Vdropout_5_50mA","Vdropout_5_300mA","Vline_regulation_5",
"Vload_regulation_33","Tss","Ven_low","Ven_high","Ien","PG_rising","PG_hys","PG_low","PG_leakage",
"PGDL_charg","PGDL_rising","PGDL_falling","PG_reaction"};
};


#endif // VALID_DATA_H
