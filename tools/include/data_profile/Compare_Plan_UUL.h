#ifndef COMPARE_PLAN_UUL_H
#define COMPARE_PLAN_UUL_H

#include "tools/include/data_profile/Attri_Unit_Ul.h"
#include "tools/include/data_profile/Unit_Ul.h"
#include <QLabel>
#include <QScrollArea>

class Compare_Plan_UUL
{
public:
    Compare_Plan_UUL();
    ~Compare_Plan_UUL();

    bool warning_head(const vector<vector<string>>& head_datas,const string& file_path);

    bool warning_show();

    void set_warning(bool flage) noexcept;
    bool get_warning() noexcept;

private:
    // 【方法区】
    void init();

    void tackle_file(const ifstream& ifs);

    // 初始化成员对象plan_uul,warning_attri_list
    void profile_test_plan(const vector<vector<string>>& all_array);

    // 构造lables
    QVector<QLabel*> profile_lables();

    // 构造mainwidget
    QWidget* get_main_widget(const QVector<QLabel*>& labels);

    // 构造scroll to show
    QScrollArea* get_scroll();


    // 【函数参数提供】
    ifstream input_file_open(const string& input_file_path);

    vector<vector<string>> tackle_file_get_all(const ifstream& ifs);

    // 将Make的源头数据的attri行中的attri列构造后返回
    vector<vector<string>> get_valid_head_datas(const vector<vector<string>>& head_datas);

    string make_label_text(const string& attri,pair<string,Unit_Ul>& test_uul,
                           const vector<pair<string,Unit_Ul>>& warnings);


    // 【对象区】
    // 存储warning的uuls【无序】【来源于datas】 // attri -> (file_path,uul)
    map<string,vector<pair<string,Unit_Ul>>> warning_uul;
    // 存储warning的pro【有序】【first来源plan,second来源datas】
    vector<pair<string,bool>> warning_attri_list;


    // 存储plan中的计划uul【来源plan】
    Attri_Unit_Ul plan_uul;

    // head valid_col div
    const string div_str = "TEST_NUM";

    // 记录是否需要show warning的bool
    bool is_warning = false;

};

#endif // COMPARE_PLAN_UUL_H
