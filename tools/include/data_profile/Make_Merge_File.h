#ifndef MAKE_MERGE_FILE_H
#define MAKE_MERGE_FILE_H

#include <vector>
#include <string>
#include <QStringList>
#include <QFileInfo>
#include "Compare_Plan_UUL.h"
using namespace std;

class Make_Merge_File
{
public:
    Make_Merge_File();
    ~Make_Merge_File();

    // 获取merge.csv，返回文件的路径
    const string make_merge_file(const QStringList& file_paths);


private:
// 【方法区】
    // 获取文件读入流
    ifstream input_file_open(const string& input_file_path);
    // 获取文件内容写出流
    ofstream output_file_open(const string& out_file_path);


    // 处理单一文件内容
    void tackle_single_file(const QString& file_path,bool first_file_flage);



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
    vector<vector<string>> get_head_datas(const vector<vector<string>>& target_datas,size_t end_head_begin_body);

    // 获取'体数据'区
    vector<vector<string>> get_body_datas(const vector<vector<string>>& target_datas,size_t end_head_begin_body);
    // 对单文件的内容进行修改获取new_body_datas
    vector<vector<string>> update_body_datas(const vector<vector<string>>& body_datas);

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
    vector<vector<string>> merge_datas;
    // 记录文件中part_id应该使用的更新数值
    int counter = 1;

    // 生成的合并文件的具体路径
    QString merge_file_path = "";


    // 生成的merge文件的文件名
    const QString merge_file_name = "merge.csv";
    // 有效数据开始的起始位置
    const string target_str = "SITE_NUM";
    // 记录目标数据的分界点
    const string div_str = "";

    // 头数据与test plan比较对象
    Compare_Plan_UUL test_plan;


};

#endif // MAKE_MERGE_FILE_H
