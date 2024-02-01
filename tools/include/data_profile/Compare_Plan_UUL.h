#ifndef COMPARE_PLAN_UUL_H
#define COMPARE_PLAN_UUL_H

#include "tools/include/data_profile/Unit_UL_Block.h"
#include "tools/include/data_profile/Unit_UL_Str.h"
#include <QLabel>
#include <QScrollArea>
#include <set>

class Compare_Plan_UUL
{
public:
    Compare_Plan_UUL();
    ~Compare_Plan_UUL();

    // 获取可能出现warning的数据
    void warning_head(const vector<vector<string>>& head_datas,const string& file_path);

    bool warning_show();

    bool warning_flage() noexcept;

    bool warning_extra_flage() noexcept;

    bool warning_extra_show();

    vector<string> get_test_plan_pro_list();

    map<string,Unit_UL_Str> get_test_plan_map();

private:
    // 【方法区】
    void init();

    void tackle_file(const ifstream& ifs);

    // 初始化成员对象plan_uul,warning_attri_list
    void profile_test_plan(const vector<vector<string>>& all_array);

    // 判断plan中的pro是否在data中不存在plan -> data
    void diff_plan_to_data(const set<string>& data_pros);

    // 构造lables
    QVector<QLabel*> profile_labels();

    // 构造mainwidget
    QWidget* get_main_widget(const QVector<QLabel*>& labels);

    // 构造extra的label
    QVector<QLabel*> make_data_less_extra_labels();

    // 构造mainwidget
    QWidget* make_data_lack_widget(const QVector<QLabel*>& labels);

    // 构造scroll to show
    QScrollArea* get_scroll();




    // 【函数参数提供】
    ifstream input_file_open(const string& input_file_path);

    vector<vector<string>> tackle_file_get_all(const ifstream& ifs);

    // 将Make的源头数据的attri行中的attri列构造后返回
    vector<vector<string>> get_valid_head_datas(const vector<vector<string>>& head_datas);

    string make_label_text(const string& attri,pair<string,Unit_UL_Str>& test_uul,
                           const vector<pair<string,Unit_UL_Str>>& warnings);


    // 【对象区】
    // 存储warning的uuls【无序】【来源于datas】 // attri -> (file_path,uul)
    map<string,vector<pair<string,Unit_UL_Str>>> warning_uul;
    // 存储warning的pro【有序】【first来源plan,second来源datas】
    vector<pair<string,bool>> warning_attri_list;


    // 存储plan中的计划uul【来源plan】
    Unit_UL_Block plan_uul; // 要与data中的字符串相同，与target获取中的数值获取不同

    // head valid_col div
    const string div_str = "TEST_NUM";

    // 记录是否需要show warning的bool
    bool is_warning = false;

    // 记录是否extra
    bool is_extra_pro = false;
    // 如果读入的data中有pro，而plan里没有【测多了】
    set<string> data_extra_pro;
    // data里没有，而plan里有【少测了pro】 // 将多出来的pros记录下来
    set<string> data_less_pro;

};

#endif // COMPARE_PLAN_UUL_H
