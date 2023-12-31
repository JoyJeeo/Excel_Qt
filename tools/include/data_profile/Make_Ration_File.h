#ifndef MAKE_RATION_FILE_H
#define MAKE_RATION_FILE_H

#include <vector>
#include <string>
#include <QString>
using namespace std;

class Make_Ration_File
{
    /*
        功能：
            生成ration.csv的源文件所需的数据，并返回
    */
public:
    Make_Ration_File();
    ~Make_Ration_File();

    const string make_ration_file(const string& timc_file_path);

    bool clear_trash_datas();

private:
// 【方法区】
    // 获取文件读入流
    ifstream input_file_open(const string& input_file_path);
    // 获取文件内容写出流
    ofstream output_file_open(const string& out_file_path);

    // 初始化timc_datas
    void init_timc_datas(const string& timc_file_path);

    // 初始化ration_datas
    void init_ration_datas();

    // 构造ration_datas
    void build_ration_datas();

    // 保存构造号的ration_datas进入ration.csv文件中
    void save_datas();


// 【函数参数分析获取】
    // 文件内容全读入对象容器中【局部对象】 // 获取all_array
    vector<vector<string>> tackle_file_get_all(const ifstream& ifs);


    // 分析timc文件获取有效数据的index位置
    // 限制下标确定函数的 指挥者
    void director_indexs();
    // 获取初始时间T0开始的位置
    void profile_end_head_begin_T0_body();
    // 获取初始时间T0结束的位置的下一位
    void profile_end_T0_body_begin_other_body();
    // 获取有效数据的列位置
    void profile_valid_col();


    // 获取并修正timc文件的头数据
    void profile_ration_head();

    // 移动T0后的数据到前面
    void move_other_data();

    // 修改数据区的数据，转换为变动率
    void update_valid_datas();

    // 判断无效的字符串空数据
    bool is_valid_number(const string& num);

    // 变动率需要调用的计算算法
    double algorithm_ration_data(double T1,double T0);

    // 判断是否只有一个partid
    bool is_solo_part();

    // 一个partid，则将其数据再拷贝一份，只改变partid部分，产生两个数据
    void copy_solo_part();


    // 获取文件的保存路径
    void profile_output_file_path() noexcept;



// 【对象区】
    // 记录原始的时刻文件数据[timc.csv]
    vector<vector<string>> timc_datas;
    // 记录目标的最终结果数据[ration.csv]
    vector<vector<string>> ration_datas;

    // 判断是否为一个时刻的数据
    bool is_solo_time = false;
    // trash数据的开始下标位置
    size_t trash_dex = 0;

    // 分析timc文件关键数据的下标位置
    size_t end_head_begin_T0_body;
    size_t end_T0_body_begin_other_body;
    size_t valid_col;

    // 生成的时刻文件的具体路径
    QString ration_file_path = "";


    // 生成的timc文件的文件名
    const QString ration_file_name = "ration.csv";

    // 记录目标数据的分界点
    const string div_str = "";
};

#endif // MAKE_RATION_FILE_H
