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

private:
// 【方法区】
    // 获取文件读入流
    ifstream input_file_open(const string& input_file_path);
    // 获取文件内容写出流
    ofstream output_file_open(const string& out_file_path);

    // 文件内容全读入对象容器中【局部对象】 // 获取all_array
    vector<vector<string>> tackle_file_get_all(const ifstream& ifs);

    // 记录目标的最终结果数据[ration_datas]
//    vector<vector<string>> m_target_datas;

    // 限制下标确定函数的 指挥者
    void director_indexs();
    // 获取初始时间T0开始的位置
    void profile_begin_row_T0();
    // 获取初始时间T0结束的位置
    void profile_end_row_T0();
    // 获取纵坐标的有效位置
    void profile_begin_col();

    // 获取文件的保存路径
    void profile_output_file_path() noexcept;

    // 保存文件
    void save_datas(const vector<vector<string>>& ration_datas);

    // 记录原始的时刻文件数据[time_datas]
    vector<vector<string>> timc_datas;

    // T0开始的位置
    int begin_row_T0;
    int end_row_T0;
    int begin_col;

    // 生成的时刻文件的具体路径
    QString ration_file_path = "";


    // 生成的timc文件的文件名
    const QString ration_file_name = "ration.csv";
};

#endif // MAKE_RATION_FILE_H
