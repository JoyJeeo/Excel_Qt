#ifndef CHART_H
#define CHART_H

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QDebug>
#include <map>
#include "My_Chart_View.h"

using namespace std;

QT_CHARTS_USE_NAMESPACE
class Chart : public QWidget
{
    Q_OBJECT

public:
    Chart(QWidget* parent = 0, QString _chartname = "折线图");
    ~Chart(){}
    void setAxis(QString _xname, qreal _xmin, qreal _xmax, int _xtickc,
                 QString _yname, qreal _ymin, qreal _ymax, int _ytickc);
    void buildChart(const vector<int>& scatter_sites,int site_max_parts,
                    const QMap<int,QVector<QPointF>>&,const pair<double,double>&);

    // 参数太多，没必要使用total_task来进行构造

private:
    // 构造数据线
    void construct_datas_series(const vector<int>& scatter_sites,int site_max_parts,
                                const QMap<int,QVector<QPointF>>& series_data,int data_series_width);
    // 构造最值线
    void construct_XI_line(const pair<double,double>& XI_line_data,int XI_series_width,
                            int site_max_parts);
    // 设置chart的图里描述样式
    void construct_legend_style(const vector<int> scatter_sites);
    QChart* qchart; // chart图表容器
    QChartView* chartview; // chart显示器容器，容纳chart图表
    //  My_Chart_View* chartview; // chart显示器容器，容纳chart图表
    QMap<int,QVector<QLineSeries*>> series; // chart图表中的数据：折线图 // 由于可能出现断点，因此需要使用这种结构进行定义
        // 横坐标代表每个site芯片【一组线】，纵坐标代表一组点，纵坐标中，是每组多个点构成的线数据
        // attri -> [site][Line_part] = [LineSeries] = many_points

    QHBoxLayout* layout; // 一个水平布局【必须有一个自己的layout才能显示控件】
    QValueAxis* axisX; // 数据轴
    QValueAxis* axisY;

    QString chartname; // chart图表名称
    //坐标轴参数
    QString xname; // x轴名称
    qreal xmin; // x轴最小值
    qreal xmax; // x轴最大值
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
//        Qt::black,Qt::blue,Qt::yellow,Qt::green,
//        Qt::darkGreen,Qt::darkCyan,Qt::darkMagenta,
//        Qt::gray,Qt::cyan,Qt::lightGray,
//        Qt::magenta,
//            Qt::darkBlue,Qt::darkRed,Qt::darkGray,Qt::darkYellow,

    }; // 线条颜色筛选
};

#endif // CHART_H
