#ifndef MAKE_TEMPERATURE_FILE_H
#define MAKE_TEMPERATURE_FILE_H

#include <string>
#include <vector>
#include <QStringList>
#include <QString>
using namespace std;

class Make_Temperature_File
{
public:
    Make_Temperature_File();
    ~Make_Temperature_File();

    // 该类处理完所有温度数据后，只需要将生成的文件路径返回即可
    const string total_task(const QStringList& file_paths);

private:

// 【方法区】
    // 获取文件读入流
    ifstream input_file_open(const string& input_file_path);

    // 获取文件内容写出流
    ofstream output_file_open(const string& out_file_path);

    // 单文件处理
    void tackle_solo_file(const QString& file_path); // 1

    // 获得单文件的文件名
    string profile_solo_file_name(const QString& file_path);

    // 分析输入的文件名【根据给定的文件名格式：*_N01_N02_..._温度(无单位).csv】
    // 以pair类型返回，first为芯片的No编号，second为所需温度
    pair<vector<string>,string> profile_siteNo_temperature(const string& file_name);

    // 文件内容全读入对象容器中【局部对象】 // 获取all_array
    vector<vector<string>> tackle_solo_file_get_all(const ifstream& ifs); // 2

    // 分析单文件的目标数据位置所需的参数
    void profile_target_data_params(vector<vector<string>>& all_array, // 由于数据为rawdata,因此可能需要修改补充其内容
                                    size_t& rows_num,
                                    size_t& cols_num,
                                    size_t& target_data_index); // 3

    // 获取目标数据【此时返回的目标数据，即为理论上不存在乱码的，补齐空位的data】
    vector<vector<string>> tackle_solo_file_get_target(const vector<vector<string>>& all_array); // 4

    // 分析头数据与体数据的分界点位置
    void profile_div_index(const vector<vector<string>>& target_datas,
                           size_t& end_head_begin_body);

    // 获取'头数据'区
    vector<vector<string>> get_head_datas(const vector<vector<string>>& target_datas,size_t end_head_begin_body);

    // 获取'体数据'区
    vector<vector<string>> get_body_datas(const vector<vector<string>>& target_datas,size_t end_head_begin_body);

    // 对单文件的内容进行修改获取new_body_datas
    vector<vector<string>> update_body_datas(const vector<vector<string>>& body_datas,
                                             const pair<vector<string>,string>& steers // 修改建议
                                             );

    // 将容器内容load进入全局数据容器中
    void load_datas(const vector<vector<string>>& datas);

    // 获取文件的保存路径
    void profile_output_file_path() noexcept;

    // 保存文件
    void save_datas();

    // 测试载入程序的数据是否符合预期
    void test_datas(const vector<vector<string>>& arrays);

// 【对象区】
    // 存储所有温度文件，修改内容后的数据容器【由rawdata中获取并修改数据后保存】【全局数据容器】
    vector<vector<string>> temperature_datas;

    // 生成的温度文件的具体路径
    QString temperature_file_path = "";


    // 生成的temperature文件的文件名
    const QString temperature_file_name = "temperatures.csv";
    // 有效数据开始的起始位置
    const string target_str = "SITE_NUM";
    // 记录目标数据的分界点
    const string div_str = "";

};

#endif // MAKE_TEMPERATURE_FILE_H
