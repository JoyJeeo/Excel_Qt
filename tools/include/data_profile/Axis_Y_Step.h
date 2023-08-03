#ifndef AXIS_Y_STEP_H
#define AXIS_Y_STEP_H

#include <string>
#include <QString>
#include <vector>
#include <map>
using namespace std;

class Axis_Y_Step
{
public:
    Axis_Y_Step();
    ~Axis_Y_Step();

    map<string,double> get_steps() noexcept;

private:
    ifstream input_file_open(const string& input_File_path);

    void profile_step_file_path() noexcept;

    void tackle_file(const ifstream& ifs);
    vector<vector<string>> tackle_file_get_all(const ifstream& ifs);
    void profile_steps(const vector<vector<string>>& all_array);

    // 获取项目的step
    void total_task();

    QString step_file_path = "";
    const QString step_file_name = "step_for_Tool.csv";

    // 记录项目对应的step
    map<string,double> m_steps;

};

#endif // AXIS_Y_STEP_H
