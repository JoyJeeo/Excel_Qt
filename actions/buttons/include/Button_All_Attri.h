#ifndef BUTTON_ALL_ATTRI_H
#define BUTTON_ALL_ATTRI_H

#include <QPushButton>
#include <QLabel>
#include "../../widgets/Widgets_Header_Proxy_01.h"

class Button_All_Attri : public QPushButton
{
    Q_OBJECT

public:
    Button_All_Attri(QWidget *parent = nullptr);
    ~Button_All_Attri();

    void total_task();

public slots:
    void task_widget_all_attri_show();

protected:
    QStringList construct_input_file_paths() noexcept;
    void construct_widgets_file_paths(const QStringList& input_file_paths,
                                    QVector<Widget_All_Attri_Show*>& widgets,
                                    QVector<QLabel*>& file_paths);
    void construct_main_widge();
    void construct_scrollarea(QWidget* widget);


    void save_pic(QWidget* saved_widget,const QString& saved_path);

    // 获取多文件
    QStringList input_file_paths;
    // 将所有窗体都存在容器中
    QVector<Widget_All_Attri_Show*> widgets;
    // 将各个窗体所属文件的名称存储起来
    QVector<QLabel*> file_paths;

    // main_widge容纳所有widget
    QWidget* main_widge;
    // 将main_widget滚动显示
    QScrollArea* scrollarea;
};
#endif // BUTTON_ALL_ATTRI_H







