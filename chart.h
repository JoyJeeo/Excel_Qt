#ifndef CHART_H
#define CHART_H

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE
class Chart : public QWidget
{
    Q_OBJECT
    QChart *qchart; // chart图表容器
    QChartView *chartview; // chart显示器容器，容纳chart图表
    QVector<QVector<QLineSeries*>> series; // chart图表中的数据：折线图

    QHBoxLayout *layout; // 一个水平布局【必须有一个自己的layout才能显示控件】
    QValueAxis *axisX; // 数据轴
    QValueAxis *axisY;

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

public:
    Chart(QWidget* parent = 0, QString _chartname = "折线图");
    ~Chart(){}
    void setAxis(QString _xname, qreal _xmin, qreal _xmax, int _xtickc,
                 QString _yname, qreal _ymin, qreal _ymax, int _ytickc);
    void buildChart(QList<QPointF> pointlist);

};

#endif // CHART_H
