#ifndef WIDGET_ALL_ATTRI_SHOW_H
#define WIDGET_ALL_ATTRI_SHOW_H

#include <QWidget>
#include "../../../tools/include/Tools_Header_Proxy_01.h"

class Widget_All_Attri_Show : public QWidget
{
    Q_OBJECT

public:
    Widget_All_Attri_Show(QWidget *parent = nullptr);
    ~Widget_All_Attri_Show();
    bool total_task();

private:
QGridLayout* pGridLayout; // 布局
File_To_Targetfile* src_file_manager; // 源文件读取分析管理器
Targetfile_Valid_Data* datas; // 目标文件读取分析管理器

    Chart*
    initChart(const string&,const vector<vector<double>>&,double,double);

    void
    while_draw();

    // 制作QList<QList<QPointF>>数据列表
    QVector<QVector<QPointF>>
    get_QVector(const vector<vector<double>>&);

    // 获取被处理的input文件名
    const string tackel_file_name();

};

#endif // WIDGET_ALL_ATTRI_SHOW_H
