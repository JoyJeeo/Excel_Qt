#ifndef CHART_CATEGORY_VALUE_H
#define CHART_CATEGORY_VALUE_H

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QDebug>
#include <map>
using namespace std;

QT_CHARTS_USE_NAMESPACE
class Chart_Category_Value : public QWidget
{
    /*
        一个相对通用的chart构造类
    */
    Q_OBJECT

public:
    Chart_Category_Value(QWidget* parent = 0, QString _chartname = "折线图",int pic_choice = 0);
    ~Chart_Category_Value();

    void setAxis(QString _xname, map<string,int>& _x_map,
                 const vector<string> &_x_map_list,int _xtickc,
                 QString _yname, qreal _ymin, qreal _ymax, int _ytickc,int pic_choice = 0,
                 const string& _x_unit = "");
    void buildChart(const vector<string> &scatter_site,
                    const vector<string> &scatter_part,
                    map<string,int> &part_map,
                    const QMap<string, QMap<string,QPointF>> &series_data,
                    const pair<double, double> &XI_proxy_data,
                    const pair<double, double> &attri_define_XI,
                    int pic_choice = 0);

private:
    // 【方法区】
    // 具体建chart的方法
    // 构造横坐标
    void build_x_axis(const string& x_unit = "");
    // 构造数据线
    void construct_datas_series(const vector<string>& scatter_site,const vector<string>& scatter_part,
                                const QMap<string, QMap<string,QPointF>> &series_data,
                                int data_series_width);
    // 构造最值线
    void construct_XI_line(const pair<double,double>& attri_define_XI,
                           int XI_series_width,const vector<string>& scatter_part,
                           map<string,int> &part_map);
    // 设置chart的图里描述样式
    void construct_legend_style(const vector<string> scatter_site,
                                const pair<double,double>& attri_define_XI,
                                int pic_choice = 0);

    // 【对象区】
    QMap<string,QVector<QLineSeries*>> series;

    QChart* qchart; // chart图表容器
    QChartView* chartview; // chart显示器容器，容纳chart图表

    QHBoxLayout* layout; // 一个水平布局【必须有一个自己的layout才能显示控件】
    QCategoryAxis* axisX; // 数据轴
    QValueAxis* axisY;

    QString chartname; // chart图表名称
    //坐标轴参数
    QString xname; // x轴名称
    map<string,int> x_map;
    vector<string> x_map_list;
    int xtickc; // x轴上的实线个数

    QString yname; // y轴名称
    qreal ymin; // y轴最小值
    qreal ymax; // y轴最大值
    int ytickc; // y轴上的实线个数

    // 设置数据线条的色彩【105组色彩】
    QVector<QColor> colors={
        QColor(91,155,213),QColor(237,125,49),QColor(165,165,165),QColor(255,192,0),QColor(0x72a351),
        QColor(12, 175, 92),QColor(224, 51, 173),QColor(45, 212, 138),QColor(103, 208, 44),QColor(76, 229, 196),
        QColor(192, 57, 220),QColor(68, 237, 115),QColor(152, 64, 196),QColor(83, 199, 150),QColor(147, 202, 42),
        QColor(51, 227, 114),QColor(195, 83, 243),QColor(29, 196, 102),QColor(150, 106, 221),QColor(56, 223, 103),
        QColor(166, 28, 209),QColor(91, 205, 64),QColor(212, 86, 177),QColor(11, 178, 91),QColor(155, 45, 219),
        QColor(35, 214, 143),QColor(103, 209, 43),QColor(78, 226, 192),QColor(199, 52, 220),QColor(63, 238, 113),
        QColor(149, 62, 195),QColor(80, 201, 148),QColor(141, 222, 36),QColor(49, 228, 116),QColor(186, 95, 233),
        QColor(14, 193, 101),QColor(164, 102, 222),QColor(54, 225, 106),QColor(176, 14, 212),QColor(101, 203, 65),
        QColor(219, 79, 181),QColor(19, 170, 83),QColor(159, 41, 214),QColor(41, 209, 136),QColor(110, 212, 38),
        QColor(83, 230, 189),QColor(203, 68, 222),QColor(70, 237, 111),QColor(161, 67, 194),QColor(96, 197, 152),
        QColor(133, 209, 26),QColor(47, 223, 102),QColor(176, 87, 239),QColor(21, 189, 105),QColor(169, 97, 226),
        QColor(59, 224, 94),QColor(188, 20, 204),QColor(113, 201, 76),QColor(224, 94, 191),QColor(26, 161, 72),
        QColor(143, 34, 201),QColor(33, 200, 126),QColor(120, 218, 8),QColor(73, 228, 189),QColor(191, 72, 213),
        QColor(66, 228, 103),QColor(183, 50, 236),QColor(88, 182, 125),QColor(123, 223, 21),QColor(19, 220, 98),
        QColor(203, 84, 210),QColor(52, 207, 127),QColor(198, 47, 192),QColor(100, 201, 89),QColor(213, 69, 207),
        QColor(82, 233, 127),QColor(154, 86, 232),QColor(45, 236, 91),QColor(180, 30, 208),QColor(112, 188, 128),
        QColor(138, 227, 7),QColor(39, 222, 84),QColor(239, 119, 212),QColor(17, 195, 108),QColor(185, 111, 221),
        QColor(64, 211, 109),QColor(170, 53, 208),QColor(80, 197, 137),QColor(202, 94, 201),QColor(72, 191, 105),
        QColor(126, 209, 72),QColor(64, 218, 62),QColor(195, 50, 226),QColor(47, 195, 111),QColor(167, 97, 221),
        QColor(92, 208, 94),QColor(189, 30, 194),QColor(105, 194, 169),QColor(221, 79, 181),QColor(63, 222, 99),
        QColor(179, 94, 231),QColor(50, 200, 117),QColor(197, 71, 202),QColor(80, 187, 109),QColor(145, 204, 47)

    }; // 线条颜色筛选
};

#endif // CHART_CATEGORY_VALUE_H
