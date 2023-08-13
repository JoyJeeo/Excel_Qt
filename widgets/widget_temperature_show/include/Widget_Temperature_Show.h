#ifndef WIDGET_TEMPERATURE_SHOW_H
#define WIDGET_TEMPERATURE_SHOW_H

#include <QWidget>
#include "tools/include/Tools_Header_Proxy_01.h"

class Widget_Temperature_Show : public QWidget
{
    Q_OBJECT

public:
    Widget_Temperature_Show(QWidget *parent = nullptr);
    ~Widget_Temperature_Show();

    // 功能四：专门用于展示温度数据的窗体
    bool total_task(const QStringList& files_path);

    // 设置窗体的title
    const string get_window_title() noexcept;

private:
    // 【方法区】
    // 循环绘制chart
    void while_draw(int row_obj_nums = 2); // 设置网格管理器默认一行的窗体个数
    // 初始化chart
    Chart_Category_Value* initChart(const string& attri, // 指定为某个属性创建chart
                     const QMap<string,QMap<string,QPointF>>& series_datas, // 设置创建chart需要的点组数据
                     const vector<string>& scatter_site,
                     const vector<string>& scatter_part,
                    int pic_choice = 0,
                     const QString& _xname = "corelation",
                                    const string& x_axis_unit = "℃");


    // 【获取程序参数】
    // 制作QList<QList<QPointF>>数据列表，[为初始化chart提供数据接口数据]
    QMap<string,QMap<string,QPointF>> get_matrix_pointF(map<string,map<string,double>>& site_part_vals,
                                                 const vector<string>& scatter_site,
                                                 const vector<string>& scatter_part
                                                 );
    // 分析定义的最值与数据最值之间的所有可能情况，将数据传出，作为二者当前情况的判断依据
    pair<double,double> profile_define_value_XI_proxy(pair<double,double> attri_define_XI,
                                                      pair<double,double> attri_value_XI
                                                      );
    // 获取y轴的最大最小值和所需的分割线个数
    void profile_y_axis_ans(const pair<double, double> &attri_value_XI,
                            const pair<double, double> &XI_proxy_data,
                            double step,
                            pair<double,double>& y_axis_around,
                            int &lines
                            );
    // 分析所需的分割线的个数
    int profile_y_axis_line_nums(const pair<double,double>& y_axis_XI,double step);

    // 【PIC文件夹内容管理】
    // 创建照片存储文件夹
    const QString build_pic_dir();
    // 清空照片文件夹中的所有内容
    void clear_pic_dir(const QString& pic_dir);

    // 【图片保存】
    // 一般情况下的图片保存
    void bulid_pic_save(Chart_Category_Value* pic_chart,
                         int chart_no, // 第几个chart
                         QString pic_dir, // 图片的存储路径
                        bool last_pic_flage = false,
                         QString pic_prefix_name = "PIC_",
                         int page_row_obj_nums = 2, // 图片中一行chart的个数[lie]
                        int page_row_nums = 3 // 一张图片的行数
                    );
    // 保存图片
    void save_pic(QWidget* saved_widget,const QString& saved_path);



    // 【对象区】
    QGridLayout* pGridLayout; // 布局
    File_To_Targetfile* src_file_manager; // 源文件读取分析管理器
    Targetfile_Temperature_Valid_Data* datas; // 目标文件读取分析管理器

    // 坐标轴的step数据
    Axis_Y_Step* y_stepor;

    // 存储分割图片的文件名
    const QString PIC_FILE_NAME = "PICS";
};

#endif // WIDGET_TEMPERATURE_SHOW_H
