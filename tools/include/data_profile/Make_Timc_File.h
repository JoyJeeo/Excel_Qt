#ifndef MAKE_TIMC_FILE_H
#define MAKE_TIMC_FILE_H

#include <vector>
#include <string>
#include <QStringList>
#include <QFileInfo>
using namespace std;

class Make_Timc_File
{
public:
    Make_Timc_File();
    ~Make_Timc_File();

    // 获取ration_file，返回timc.csv文件的路径
    const string make_timc_file(const QStringList& dir_paths);


private:
// 【方法区】
    // 获取文件读入流
    ifstream input_file_open(const string& input_file_path);
    // 获取文件内容写出流
    ofstream output_file_open(const string& out_file_path);


    // 处理单一文件夹内容
    void tackle_single_dir(const QString& dir_path,bool& first_file_flage);
    // 处理单一文件内容
    void tackle_single_file(const string& time_name,const QString& file_path,
                            bool& first_file_flage);



    // 【获取函数参数的函数】
    // 获取时刻名称
    string profile_dir_name(const QString& dir_path);
    // 获取文件夹内需要的文件内容，并按文件名大小排序，获取文件夹内文件信息
    QList<QFileInfo> profile_dir_inner_file_infos(const QString& dir_path,
                                                  const QString& need_files = "*.csv");
    // 获取文件夹内文件路径
    QStringList profile_dir_inner_file_paths(const QList<QFileInfo>& file_infos);

    // 获取修改后的文件名称No_...【命名规范以'N数字'的形式命名文件夹内文件的名称】
    string profile_no_name(const QString& file_path);

// 【分析名称需要的参数函数】 //
    // 获取任意文件的'/'后的名称
    string profile_path_to_name(const QString& path);
    // 获取下划线的位置
    int profile_underline_index(const string& name);
    // 获取文件名的.的下标位置
    int profile_nameDot_index(const string& name);
//////////////////////////////////////////////

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

    // 对单文件的内容进行修改获取new_body_data 【只处理并返回，体数据中的最后一次数据结果】
    vector<vector<string>> update_body_datas(const vector<vector<string>>& body_datas,
                                             // first为No_...,second为时刻名称
                                             const pair<string,string>& steer // 修改建议
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
    // 存储处理好的时刻文件信息
    vector<vector<string>> time_datas;

    // 生成的时刻文件的具体路径
    QString timc_file_path = "";


    // 生成的timc文件的文件名
    const QString timc_file_name = "timc.csv";
    // 有效数据开始的起始位置
    const string target_str = "SITE_NUM";
    // 记录目标数据的分界点
    const string div_str = "";

};

#endif // MAKE_TIMC_FILE_H
