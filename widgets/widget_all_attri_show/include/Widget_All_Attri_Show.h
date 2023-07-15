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

    // 获取源文件的文件名
    void set_src_file_name(const string& str) noexcept;
    const string get_src_file_name() noexcept;

    bool total_task();

private:
QGridLayout* pGridLayout; // 布局
File_To_Targetfile* src_file_manager; // 源文件读取分析管理器
Targetfile_Valid_Data* datas; // 目标文件读取分析管理器

    Chart*
    initChart(const string&,const vector<vector<double>>&,double,double);

    void
    while_draw(int row_obj_nums = 2); // 设置网格管理器默认一行的窗体个数

    void
    build_axis(Chart* chart);

    // 制作QList<QList<QPointF>>数据列表
    QVector<QVector<QPointF>>
    get_QVector(const vector<vector<double>>&);

    // 获取被处理的input文件名
    const string tackel_file_name();

    string src_file_name;

};

#endif // WIDGET_ALL_ATTRI_SHOW_H
