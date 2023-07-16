#ifndef MY_CHART_VIEW_H
#define MY_CHART_VIEW_H

#include <QChartView>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE // QChartView是包在命名空间内部的类，QT_CHARTS_USE_NAMESPACE将这个命名空间展开即可使用

class My_Chart_View : public QChartView
{
public:
    explicit My_Chart_View(QWidget *parent = nullptr);
    explicit My_Chart_View(QChart *chart, QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
signals:
    void sgl_recoverRange(My_Chart_View *p);

private:
    bool is_Pressed_;
};

#endif // MY_CHART_VIEW_H
