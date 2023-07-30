#ifndef MAKE_RATION_FILE_H
#define MAKE_RATION_FILE_H

#include <vector>
#include <string>
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

    vector<vector<string>> make_ration_file(vector<vector<string>> time_datas);

private:

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

    // 记录原始的时刻文件数据[time_datas]
    vector<vector<string>> m_source_datas;

    // T0开始的位置
    int begin_row_T0;
    int end_row_T0;
    int begin_col;

};

#endif // MAKE_RATION_FILE_H
