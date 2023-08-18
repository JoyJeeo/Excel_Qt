#ifndef MAKE_NORMAL_FILE_H
#define MAKE_NORMAL_FILE_H

#include <vector>
#include <string>
#include <QStringList>
#include <QFileInfo>
#include "Compare_Plan_UUL.h"
using namespace std;

class Make_Normal_File
{
public:
    Make_Normal_File();
    ~Make_Normal_File();

    // 获取merge.csv，返回文件的路径
    const string make_normal_file(const QString& file_path);


private:
// 【方法区】
    // 获取文件读入流
    ifstream input_file_open(const string& input_file_path);
    // 获取文件内容写出流
    ofstream output_file_open(const string& out_file_path);


    // 处理单一文件内容
    void tackle_file(const QString& file_path);



    // 【获取函数参数的函数】
    // 文件内容全读入对象容器中【局部对象】 // 获取all_array
    vector<vector<string>> tackle_solo_file_get_all(const ifstream& ifs);

    // 分析单文件的目标数据位置所需的参数
    void profile_target_data_params(vector<vector<string>>& all_array, // 由于数据为rawdata,因此可能需要修改补充其内容
                                    size_t& rows_num,
                                    size_t& cols_num,
                                    size_t& target_data_index);

    // 获取目标数据【此时返回的目标数据，即为理论上不存在乱码的，补齐空位的data】
    vector<vector<string>> tackle_solo_file_get_target(vector<vector<string>>& all_array, // 可能需要补""空，不能const传入
                                                       size_t rows_num,
                                                       size_t cols_num,
                                                       size_t target_data_index);

    // 分析头数据与体数据的分界点位置
    void profile_div_index(const vector<vector<string>>& target_datas,
                           size_t& end_head_begin_body);

    // 获取'头数据'区
    vector<vector<string>> get_head_datas(const vector<vector<string>>& target_datas,
                                          size_t end_head_begin_body);

    // 获取'体数据'区
    vector<vector<string>> get_body_datas(const vector<vector<string>>& target_datas,
                                          size_t end_head_begin_body);

    // 将容器内容load进入全局数据容器中
    void load_datas(const vector<vector<string>>& datas);

    // 获取文件的保存路径
    void profile_output_file_path() noexcept;

    // 保存文件
    void save_datas();

    // 测试载入程序的数据是否符合预期
    void test_datas(const vector<vector<string>>& arrays);


// 【对象区】
    // 存储处理好的合并文件信息
    vector<vector<string>> normal_datas;

    // 生成的合并文件的具体路径
    QString normal_file_path = "";


    // 生成的merge文件的文件名
    const QString normal_file_name = "normal.csv";
    // 有效数据开始的起始位置
    const string target_str = "SITE_NUM";
    // 记录目标数据的分界点
    const string div_str = "";

    // 头数据与test plan比较对象
    Compare_Plan_UUL test_plan;

};

#endif // MAKE_NORMAL_FILE_H
