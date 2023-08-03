#ifndef WIDGET_ALL_ATTRI_SHOW_H
#define WIDGET_ALL_ATTRI_SHOW_H

#include <QWidget>
#include "tools/include/Tools_Header_Proxy_01.h"

class Widget_All_Attri_Show : public QWidget
{
    Q_OBJECT

public:
    Widget_All_Attri_Show(QWidget *parent = nullptr);
    ~Widget_All_Attri_Show();

    // 获取源文件的文件名
    void set_src_file_name(const string& str) noexcept;
    const string get_src_file_name() noexcept;

    // 设置窗体的title
    const string get_window_title() noexcept;

    vector<int> get_scatter_sites() noexcept;
    int get_site_max_parts() noexcept;

    // 【功能三】
    vector<string> get_scatter_time_sites() noexcept;
    int get_time_site_max_parts() noexcept;

    bool total_task(const string& input_file_path);
    // 合并单文件的窗体
    bool merge_task(const QStringList& files_path);
    // 【功能三】时刻文件的分析窗体
    bool time_task(const QStringList& dir_path);

private:
    // 循环绘制折线图chart
    void while_draw(int row_obj_nums = 2); // 设置网格管理器默认一行的窗体个数
    // 初始化chart
    Chart* initChart(const string& attri, // 指定为某个属性创建chart
                     const QMap<int,QVector<QPointF>>& point_vecs, // 设置创建chart需要的点组数据
                     const vector<int>& scatter_sites,
                     int site_max_parts,
                     double axisX_k = 1, // 设置X轴的缩放大小
                     double axisY_k = 0.01, // 设置Y轴的缩放大小
                    int choice = 0
                );

    void save_pic(QWidget* saved_widget,const QString& saved_path);

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
    QMap<int,QVector<QPointF>> get_matrix_pointF(map<int,vector<double>>& site_part_vals,
                                                 const vector<int>& scatter_sites,
                                                 int site_max_parts);

    // 获取被处理的input文件名
    const string tackel_file_name();
//    // 记录scatter_sites，由内部维护
//    void profile_scatter_sites();
//    // 记录芯片的最大轮次
//    void profile_site_max_parts();
//    // 记录芯片列表数据
//    void profile_site_list();

    // 【功能三】
//     记录scatter_time_sites，由内部维护
//    void profile_scatter_time_sites();
//    // 记录芯片的最大轮次
//    void profile_time_site_max_parts();
    // 循环绘制折线图chart
    void time_while_draw(int row_obj_nums = 2);
    // 制作QList<QList<QPointF>>数据列表，[为初始化chart提供数据接口数据]
    QMap<string,QVector<QPointF>> get_time_matrix_pointF(map<string,vector<double>>& time_datas,
                                                         const vector<string>& scatter_time_sites,
                                                         int time_site_max_parts);
    // 初始化chart
    Chart* time_initChart(const string& attri, // 指定为某个属性创建chart
                    const QMap<string,QVector<QPointF>>& time_points, // 设置创建chart需要的点组数据
                    Targetfile_Valid_Data* Datas,
                      const vector<string>& scatter_time_sites,
                      int time_site_max_parts,
                     double axisX_k = 1, // 设置X轴的缩放大小
                     double axisY_k = 0.01, // 设置Y轴的缩放大小
                    int choice = 0);
    QString profile_time_attri_unit(const string& attri,Targetfile_Valid_Data* Datas);
    // 获取数据线的最值线数据
    pair<double,double> profile_time_data_series_XI(const string& attri,Targetfile_Valid_Data* Datas,double zoom = 0.05);
    // 获取缩放后的最值数据【基本通用】
    double profile_time_zoom_dist_XI(const pair<double,double>& time_data_XI);
    // 获取纵坐标的最值数据
    pair<double,double> profile_time_Y_XI(const pair<double,double>& data_series_XI,double zoom_dist,double axisY_k);
    // 【美化纵坐标范围】：综合数据线最值和纵坐标最值获取综合最值，美化初始化的曲线图
    pair<double,double> profile_time_generalY_XI(const pair<double,double>& time_data_series_XI,
                                            const pair<double,double>& Y_XI);

    // 创建照片存储文件夹
    const QString build_pic_dir();
    // 清空照片文件夹中的所有内容
    void clear_pic_dir(const QString& pic_dir);

    // test
    void test_for(QMap<int,QVector<QPointF>>& site_part_vals,
                  const vector<int>& scatter_sites,
                  int site_max_parts);


    // 【这几个对象，必须存在于窗体这个类内定义，而不是其他类内，因为这是专属于这个窗体的功能，而不是其他窗体的功能】
    QGridLayout* pGridLayout; // 布局
    File_To_Targetfile* src_file_manager; // 源文件读取分析管理器
    Targetfile_Valid_Data* datas; // 目标文件读取分析管理器

    // 【功能三】
//    vector<string> scatter_time_sites;
//    int time_site_max_parts;
    // 专门获取ration文件内容的target文件分析对象
    Targetfile_Valid_Data* ration_datas;

    // 【不应该出现在窗体中，因为一个窗体中，可能会有很多不同的chart，而不唯一】
    // 所有芯片的site
//    vector<int> scatter_sites;
    // 芯片列表 site_list
//    vector<int> site_list;
    // 芯片的最大轮次，其一定为该类的属性，但不一定是画图中的横坐标
//    int site_max_parts;

    string src_file_name;

    // 存储分割图片的文件名
    const QString PIC_FILE_NAME = "PICS";
};

#endif // WIDGET_ALL_ATTRI_SHOW_H
