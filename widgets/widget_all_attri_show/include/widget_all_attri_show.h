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
    void draw();

private:
Chart *chart;
QGridLayout *pGridLayout; // 布局

    Chart*
    initChart(const string&,const vector<vector<double>>&,Valid_Data&,double,double);

    void
    while_draw(const Valid_Data);

    // 制作QList<QList<QPointF>>数据列表
    QVector<QVector<QPointF>>
    get_QVector(const vector<vector<double>>&);

};

#endif // WIDGET_ALL_ATTRI_SHOW_H
