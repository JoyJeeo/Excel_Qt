#include "tools/include/data_profile/Compare_Plan_UUL.h"

#include <QCoreApplication>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

// 全局的test_plan文件的地址
extern QString test_plan_path; // test plan的路径【test plan中的数据格式是绝对的，文件名称不是绝对的】

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

void Compare_Plan_UUL::warning_head(const vector<vector<string>> &head_datas,
                                    const string& file_path)
{
    /*
        说明：
            1. data pro == plan pro 显示是否存在plan数据上的差异 // 具体到数据出现问题的文件中
            2. data pro > plan pro 显示extra的项目
            3. data pro < plan pro 显示less的项目 // 这两个只会显示出现问题的pro，而不会显示具体在哪个文件中
            4. 都可以显示，如果出现这三种情况中的任意一种时

    */
    try {
        vector<vector<string>> valid_head = get_valid_head_datas(head_datas);

        // 分析是否有与test plan不同的属性
        size_t attri_row = 0;
        size_t unit_row = 1;
        size_t limitl_row = 2;
        size_t limitU_row = 3;

        // 获取data的head pro映射集
        set<string> data_pros;

        for(size_t col = 0;col < valid_head[attri_row].size();col++)
        {
            string attri = valid_head[attri_row][col];
            Unit_UL_Str uul(valid_head[unit_row][col],
                        valid_head[limitl_row][col],
                        valid_head[limitU_row][col]);

            // 添加到data的映射中
            data_pros.insert(attri);

            // 无warning
            map<string,Unit_UL_Str>::iterator item_iterator = plan_uul.m_attri_uuls.find(attri);
            if(item_iterator != plan_uul.m_attri_uuls.end() && uul == item_iterator->second)
                continue;

            // warning
            // 这个属性没被添加过
            if(warning_uul.find(attri) == warning_uul.end())
            {
                auto obj = find_if(warning_attri_list.begin(),warning_attri_list.end(),
                                 [&attri](const pair<string,bool>& a){
                                    return a.first == attri;
                                });
                // 在test plan中没找到
                if(obj == warning_attri_list.end())
                {
                    // 没有项目，则报告 // 一个pro只会被添加一次
                    if(data_extra_pro.find(attri) == data_extra_pro.end())
                    {
                        // plan欠缺的pro
                        data_extra_pro.insert(attri);

                        // 设置标志
                        is_extra_pro = true;
                    }

                    // 记录中存在，则继续循环
                    continue;
                }
                // 找到了，则warning
                is_warning = true;
                // 设置搜索列表
                obj->second = true;
                // 初始化
                warning_uul.insert(make_pair(attri,vector<pair<string,Unit_UL_Str>>()));
                // 添加waring uul
                warning_uul[attri].push_back(make_pair(file_path,uul));
            }
            // 添加过
            else {
                warning_uul[attri].push_back(make_pair(file_path,uul));
            }

        }

        // 检测plan的pro 与 data的pro的缺少关系
        diff_plan_to_data(data_pros);

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::warning_head";
        throw;
    }
}

bool Compare_Plan_UUL::warning_show()
{
    try {
        QVector<QLabel*> labels = profile_labels();
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

bool Compare_Plan_UUL::warning_flage() noexcept
{
    return is_warning;
}

bool Compare_Plan_UUL::warning_extra_flage() noexcept
{
    return is_extra_pro;
}

bool Compare_Plan_UUL::warning_extra_show()
{
    try {
        QVector<QLabel*> labels = make_data_less_extra_labels();
        QWidget* lack_window = make_data_lack_widget(labels);
        QScrollArea* scroll = get_scroll();

        scroll->setWidget(lack_window);
        scroll->show();

        return true;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::warning_extra_show";
        throw;
    }
}

vector<string> Compare_Plan_UUL::get_test_plan_pro_list()
{
    return plan_uul.m_attri_list;
}

map<string, Unit_UL_Str> Compare_Plan_UUL::get_test_plan_map()
{
    return plan_uul.m_attri_uuls;
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
            string pro_name = all_array[row_attri][col_attri];
            plan_uul.m_attri_list.push_back(pro_name);
            plan_uul.m_attri_uuls.insert(make_pair(pro_name,
                                                   Unit_UL_Str(all_array[row_attri][col_unit],
                                                           all_array[row_attri][col_limitL],
                                                           all_array[row_attri][col_limitU]
                                                           ))); // 【！！！与target的头数据分析分开,string】
            warning_attri_list.push_back(make_pair(all_array[row_attri][col_attri],false));
        }

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::profile_test_plan";
        throw;
    }
}

void Compare_Plan_UUL::diff_plan_to_data(const set<string> &data_pros)
{
    try {

        for(const pair<string,bool>& x : warning_attri_list)
        {
            // 以plan的pro为标准，查看plan -> data中的缺少的attri
            if(data_pros.find(x.first) == data_pros.end())
            {
                // 设置标志
                is_extra_pro = true;

                // 是否已经被添加过【唯一添加】
                if(data_less_pro.find(x.first) == data_less_pro.end())
                {
                    data_less_pro.insert(x.first);
                }
            }
        }


    } catch (...) {
        qDebug() << "Compare_Plan_UUL::diff_plan_to_data";
        throw;
    }
}

QVector<QLabel *> Compare_Plan_UUL::profile_labels()
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
            pair<string,Unit_UL_Str> test_uul = make_pair("Test_Plan: ",plan_uul.m_attri_uuls[attri]);
            vector<pair<string,Unit_UL_Str>>& warnings = warning_uul[attri];

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
        QWidget *main_widge = new QWidget(nullptr);
        QVBoxLayout* v_layout = new QVBoxLayout(main_widge);
        main_widge->setLayout(v_layout);

        QString div_line = "";
        for(int i = 0;i < 200;i++)
        {
            div_line += "=";
        }


        for(int i = 0;i < labels.size();i++)
        {
            v_layout->addWidget(labels[i]);

            QLabel* null_label = new QLabel(div_line,nullptr);
            v_layout->addWidget(null_label);
        }

        return main_widge;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::get_main_widget";
        throw;
    }
}

QVector<QLabel *> Compare_Plan_UUL::make_data_less_extra_labels()
{
    try {
        QLabel* label = nullptr;
        QVector<QLabel *> labels;

        // 设置字体
        QFont font = QFont("Consolas");
        font.setStyleStrategy(QFont::PreferAntialias);
        font.setPixelSize(20);
        font.setBold(true);

        // data中less的pro【少测的pro】
        if(data_less_pro.size() > 0)
        {
            // 组装QLabel中的字符串内容
            label = new QLabel;
            label->setFont(font);

            QString title = "Items in the test plan are missing from the read-in items\n";
            QString List = "List:\n";

            QString text = "";

            size_t counter = 1;
            for(string pro : data_less_pro)
            {
                text += QString::fromStdString(pro +
                                               (counter == data_less_pro.size() ? "" :
                                                    counter % 4 == 0 ? "\n" : "  |  ")
                                               );
                counter++;
            }

            // 写入label
            label->setText(title + List + text);
            // 载入labels
            labels.push_back(label);
        }

        // data中extra的pro【多测的pro，plan中不需要测的项目】
        if(data_extra_pro.size() > 0)
        {
            // 组装QLabel中的字符串内容
            label = new QLabel;
            label->setFont(font);

            QString title = "Among the items measured in the read-in file, "
                            "the items measured more than the test plan\n";
            QString List = "List:\n";

            QString text = "";

            size_t counter = 1;
            for(string pro : data_extra_pro)
            {
                text += QString::fromStdString(pro +
                                               (counter == data_extra_pro.size() ? "" :
                                                    counter % 4 == 0 ? "\n" : "  |  ")
                                               );
                counter++;
            }

            // 写入label
            label->setText(title + List + text);
            // 载入labels
            labels.push_back(label);
        }


        return labels;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::make_data_less_extra_labels";
        throw;
    }
}

QWidget *Compare_Plan_UUL::make_data_lack_widget(const QVector<QLabel *> &labels)
{
    try {
        // 容纳所有label
        QWidget *main_widge = new QWidget(nullptr);
        QHBoxLayout* H_layout = new QHBoxLayout(main_widge);
        main_widge->setLayout(H_layout);


        for(int i = 0;i < labels.size();i++)
        {
            H_layout->addWidget(labels[i]);
        }

        return main_widge;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::make_data_lack_widget";
        throw;
    }
}

QScrollArea *Compare_Plan_UUL::get_scroll()
{
    try {
        QScrollArea* scrollarea = new QScrollArea(nullptr);
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
        size_t end_row = 4;

        for(col_begin_attri = 0; col_begin_attri < head_datas[row].size();col_begin_attri++)
        {
            if(head_datas[row][col_begin_attri] == this->div_str)
            {
                col_begin_attri++;
                break;
            }
        }

        vector<vector<string>> valid_head_datas;

        // 头中的空行不需要
        for(size_t i = 0;i < end_row;i++)
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

string Compare_Plan_UUL::make_label_text(const string &attri, pair<string, Unit_UL_Str> &test_uul,
                                         const vector<pair<string, Unit_UL_Str>> &warnings)
{
    try {
        string str =
                attri + "\n" +
                test_uul.first + string(test_uul.second) + "\n";

        // 循环添加warning
        for(size_t i = 0;i < warnings.size();i++)
        {
            pair<string, Unit_UL_Str> t = warnings[i];
            str += t.first + ":\n\t" +
                    string(t.second) +
                    (i != warnings.size() - 1 ? "\n" : "");
        }

        return str;

    } catch (...) {
        qDebug() << "Compare_Plan_UUL::make_label_text";
        throw;
    }
}
