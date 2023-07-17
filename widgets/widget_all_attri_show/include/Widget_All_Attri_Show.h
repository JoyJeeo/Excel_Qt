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
    // 循环绘制折线图chart
    void while_draw(int row_obj_nums = 2); // 设置网格管理器默认一行的窗体个数
    // 初始化chart
    Chart* initChart(const string& attri, // 指定为某个属性创建chart
                     const QVector<QVector<QPointF>>& point_vecs, // 设置创建chart需要的点组数据
                     double axisX_k = 1, // 设置X轴的缩放大小
                     double axisY_k = 0.01 // 设置Y轴的缩放大小
                    );

    // 设置纵坐标和最值线的数据，[为构造chart提供接口数据]
    // 获取属性单位
    QString profile_attri_unit(const string& attri);
    // 获取数据线的最值线数据
    pair<double,double> profile_data_series_XI(const string& attri,double zoom = 0.05);
    // 获取缩放后的最值数据【基本通用】
    double profile_zoom_dist_XI(const pair<double,double>& data_XI);
    // 获取纵坐标的最值数据
    pair<double,double> profile_Y_XI(const pair<double,double>& data_series_XI,double zoom_dist,double axisY_k);
    // 【美化纵坐标范围】：综合数据线最值和纵坐标最值获取综合最值，美化初始化的曲线图
    pair<double,double> profile_generalY_XI(const pair<double,double>& data_series_XI,
                                            const pair<double,double>& Y_XI);

    // 制作QList<QList<QPointF>>数据列表，[为初始化chart提供数据接口数据]
    QVector<QVector<QPointF>> get_QVector(const vector<vector<double>>&);

    // 获取被处理的input文件名
    const string tackel_file_name();


    // 【这几个对象，必须存在于窗体这个类内定义，而不是其他类内，因为这是专属于这个窗体的功能，而不是其他窗体的功能】
    QGridLayout* pGridLayout; // 布局
    File_To_Targetfile* src_file_manager; // 源文件读取分析管理器
    Targetfile_Valid_Data* datas; // 目标文件读取分析管理器

    string src_file_name;
};

#endif // WIDGET_ALL_ATTRI_SHOW_H
