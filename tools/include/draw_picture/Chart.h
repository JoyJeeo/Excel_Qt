#ifndef CHART_H
#define CHART_H

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QDebug>
#include <map>
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
    void buildChart(const QVector<QVector<QPointF>>&,const pair<double,double>&);

private:
    QChart* qchart; // chart图表容器
    QChartView* chartview; // chart显示器容器，容纳chart图表
    QVector<QVector<QLineSeries*>> series; // chart图表中的数据：折线图 // 由于可能出现断点，因此需要使用这种结构进行定义
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

    QVector<int> colors={
        Qt::gray,Qt::blue,Qt::yellow,Qt::green,
        Qt::darkGreen,Qt::darkCyan,Qt::darkMagenta,
        Qt::black,Qt::cyan,Qt::lightGray,
        Qt::magenta,
            Qt::darkBlue,Qt::darkRed,Qt::darkGray,Qt::darkYellow,

    }; // 线条颜色筛选
};

#endif // CHART_H
