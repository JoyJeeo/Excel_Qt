#ifndef BUTTON_TIME_COMPARE_H
#define BUTTON_TIME_COMPARE_H

#include <QPushButton>
#include <QLabel>
#include "widgets/Widgets_Header_Proxy_01.h"

class Button_Time_Compare : public QPushButton
{
    Q_OBJECT

public:
    Button_Time_Compare(QWidget *parent = nullptr);
    ~Button_Time_Compare();

    void total_task();

public slots:
    void task_widget_time_file_comp_show();

protected:
    QStringList construct_input_dir_paths() noexcept;
    void construct_widgets_dir_paths();
    void construct_main_widge();
    void construct_scrollarea();


    void save_pic(QWidget* saved_widget,const QString& saved_path);

    // 这些对象，都是该按钮下触发槽后需要使用的属性对象
    // 获取多文件
    QStringList input_dir_paths;
    // 将所有窗体都存在容器中
    QVector<Widget_All_Attri_Show*> widgets;
    // 将各个窗体所属文件的名称存储起来
    QVector<QLabel*> file_paths;

    // main_widge容纳所有widget
    QWidget* main_widge;
    // 将main_widget滚动显示
    QScrollArea* scrollarea;
};

#endif // BUTTON_TIME_COMPARE_H
