#include "tools/include/data_profile/Compare_Plan_UUL.h"

#include <QCoreApplication>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QVBoxLayout>

// 全局的test_plan文件的地址
QString test_plan_path = ""; // test plan的路径
bool is_first_testplan = true; // test plan 是否被初始化【谁是第一个窗体】

Compare_Plan_UUL::Compare_Plan_UUL()
{
    try {

        // 每次都重新读取test plan
        init();

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::Compare_Plan_UUL";
        throw;
    }
}

Compare_Plan_UUL::~Compare_Plan_UUL()
{

}

bool Compare_Plan_UUL::warning_head(const vector<vector<string>> &head_datas,
                                    const string& file_path)
{
    try {
        vector<vector<string>> valid_head = get_valid_head_datas(head_datas);

        // 分析是否有与test plan不同的属性
        size_t attri_row = 0;
        size_t unit_row = 1;
        size_t limitl_row = 2;
        size_t limitU_row = 3;

        for(size_t col = 0;col < valid_head[attri_row].size();col++)
        {
            string attri = valid_head[attri_row][col];
            Unit_Ul uul(valid_head[unit_row][col],
                        stod(valid_head[limitl_row][col]),
                        stod(valid_head[limitU_row][col]));

            // 无warning
            if(uul != plan_uul.m_attri_uuls[attri]) return false;

            // warning
            // 这个属性没被添加过
            if(warning_uul.find(attri) == warning_uul.end())
            {
                auto obj = find_if(warning_attri_list.begin(),warning_attri_list.end(),
                                 [&attri](const pair<string,bool>& a){
                                    return a.first == attri;
                                });

                // 设置搜索列表
                obj->second = true;
                // 初始化
                warning_uul.insert(make_pair(attri,vector<pair<string,Unit_Ul>>()));
                // 添加waring uul
                warning_uul[attri].push_back(make_pair(file_path,uul));
            }
            // 添加过
            else {
                warning_uul[attri].push_back(make_pair(file_path,uul));
            }

        }

        return true;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::warning_head";
        throw;
    }
}

bool Compare_Plan_UUL::warning_show()
{
    try {
        QVector<QLabel*> labels = profile_lables();
        QWidget* warning_window = get_main_widget(labels);
        QScrollArea* scroll = get_scroll();

        scroll->setWidget(warning_window);
        scroll->show();

        return true;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::warning_show";
        throw;
    }
}

void Compare_Plan_UUL::set_warning(bool flage) noexcept
{
    is_warning = flage;
}

bool Compare_Plan_UUL::get_warning() noexcept
{
    return is_warning;
}

void Compare_Plan_UUL::init()
{
    try {
        ifstream ifs = input_file_open(test_plan_path.toStdString());

        tackle_file(ifs);

        ifs.close();

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::init";
        throw;
    }
}

void Compare_Plan_UUL::tackle_file(const ifstream &ifs)
{
    try {
        vector<vector<string>> test_plan = tackle_file_get_all(ifs);
        profile_test_plan(test_plan);

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::tackle_file";
        throw;
    }
}

void Compare_Plan_UUL::profile_test_plan(const vector<vector<string>> &all_array)
{
    try {
        size_t row_attri = 1;
        size_t col_attri = 0;
        size_t valid_col = 1;
        // size_t end_col = 4;

        // 分析uul列
        int invalid_row = 0;
        size_t col_unit =
                all_array[invalid_row][valid_col] == plan_uul.m_Unit ?
                    valid_col : all_array[invalid_row][valid_col + 1] == plan_uul.m_Unit ?
                    valid_col + 1 : valid_col + 2;
        size_t col_limitL =
                all_array[invalid_row][valid_col + 1] == plan_uul.m_LimitL ?
                    valid_col + 1 : all_array[invalid_row][valid_col] == plan_uul.m_LimitL ?
                    valid_col : valid_col + 2;
        size_t col_limitU =
                all_array[invalid_row][valid_col + 2] == plan_uul.m_LimitU ?
                    valid_col + 2 : all_array[invalid_row][valid_col] == plan_uul.m_LimitU ?
                    valid_col : valid_col + 1;


        // 填充uul
        for(;row_attri < all_array.size();row_attri++)
        {
            plan_uul.m_attri_uuls.insert(make_pair(all_array[row_attri][col_attri],
                                                   Unit_Ul(all_array[row_attri][col_unit],
                                                           stod(all_array[row_attri][col_limitL]),
                                                           stod(all_array[row_attri][col_limitU])
                                                           ))); // 【！！！与target的头数据分析分开,string】
            warning_attri_list.push_back(make_pair(all_array[row_attri][col_attri],false));
        }

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::profile_test_plan";
        throw;
    }
}

QVector<QLabel *> Compare_Plan_UUL::profile_lables()
{
    try {
        string attri = "";
        QLabel* label = nullptr;
        QVector<QLabel *> labels;

        // 设置字体
        QFont font = QFont("Consolas");
        font.setStyleStrategy(QFont::PreferAntialias);
        font.setPixelSize(20);
        font.setBold(true);

        for(size_t i = 0;i < warning_attri_list.size();i++)
        {
            // 没找到
            if(!warning_attri_list[i].second)
                continue;


            attri = warning_attri_list[i].first;
            pair<string,Unit_Ul> test_uul = make_pair("Test_Plan: ",plan_uul.m_attri_uuls[attri]);
            vector<pair<string,Unit_Ul>>& warnings = warning_uul[attri];

            // 组装QLabel中的字符串内容
            label = new QLabel;
            string text = make_label_text(attri,test_uul,warnings);
            label->setText(QString::fromStdString(text));
            label->setFont(font);

            // 加入labels
            labels.push_back(label);
        }

        return labels;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::profile_lables";
        throw;
    }
}

QWidget *Compare_Plan_UUL::get_main_widget(const QVector<QLabel *> &labels)
{
    try {
        // 容纳所有label
        QWidget *main_widge = new QWidget;
        QVBoxLayout* v_layout = new QVBoxLayout(main_widge);
        main_widge->setLayout(v_layout);

        for(int i = 0;i < labels.size();i++)
        {
            v_layout->addWidget(labels[i]);
        }

        return main_widge;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::get_main_widget";
        throw;
    }
}

QScrollArea *Compare_Plan_UUL::get_scroll()
{
    try {
        QScrollArea* scrollarea = new QScrollArea;
        scrollarea->resize(800,620);
        scrollarea->setWindowTitle("Test_Plan WARNING");
        scrollarea->setWidgetResizable(true);
        scrollarea->setAttribute(Qt::WA_DeleteOnClose);

        return scrollarea;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::get_scroll";
        throw;
    }
}

ifstream Compare_Plan_UUL::input_file_open(const string &input_file_path)
{
    try {
        ifstream ifs;
        ifs.open(input_file_path);
        if(!ifs.is_open())
        {
            cerr<<"test_plan file open fail"<<endl;
            exit(1);
        }
        return ifs;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::input_file_open";
        throw;
    }
}

vector<vector<string>> Compare_Plan_UUL::tackle_file_get_all(const ifstream &ifs)
{
    try {
        string s;
        vector<vector<string>> all_array;
        vector<string> arrays;
        string array;
        ifstream& t_ifs = const_cast<ifstream&>(ifs);
        while(getline(t_ifs,s))
        {
            istringstream sin(s);

            while(getline(sin,array,','))
            {
                // cout<<array.size()<<" ";
                arrays.push_back(array);
            }
            // cout<<endl;
            all_array.push_back(arrays);
            arrays.clear();
        }

        return all_array;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::tackle_file_get_all";
        throw;
    }
}

vector<vector<string>> Compare_Plan_UUL::get_valid_head_datas(const vector<vector<string>>&
                                                              head_datas)
{
    try {
        size_t row = 0;
        size_t col_begin_attri;

        for(col_begin_attri = 0; col_begin_attri < head_datas[row].size();col_begin_attri++)
        {
            if(head_datas[row][col_begin_attri] == this->div_str)
            {
                col_begin_attri++;
                break;
            }
        }

        vector<vector<string>> valid_head_datas;

        for(size_t i = 0;i < head_datas.size();i++)
        {
            valid_head_datas.push_back(vector<string>(
                                              head_datas[i].begin() + col_begin_attri,
                                              head_datas[i].end()));
        }

        return valid_head_datas;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::get_valid_head_datas";
        throw;
    }
}

string Compare_Plan_UUL::make_label_text(const string &attri, pair<string, Unit_Ul> &test_uul,
                                         const vector<pair<string, Unit_Ul>> &warnings)
{
    try {
        string str =
                attri + "\n" +
                test_uul.first + ": " +  string(test_uul.second) + "\n";

        // 循环添加warning
        for(size_t i = 0;i < warnings.size();i++)
        {
            pair<string, Unit_Ul> t = warnings[i];
            str += t.first + ":\n  " +
                    string(t.second) +
                    (i != warnings.size() - 1 ? "\n" : "");
        }


    } catch (...) {
        qDebug() << "Compare_Plan_UUL::make_label_text";
        throw;
    }
}
