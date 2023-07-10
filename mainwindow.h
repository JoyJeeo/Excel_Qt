#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chart.h"
#include "HEADER_01.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Chart *chart;
    QGridLayout *pGridLayout; // 布局

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void draw();

private:
    Ui::MainWindow *ui;

    Chart* initChart(const string&,const vector<vector<double>>&,Valid_Data&,double,double);
    void while_draw(const Valid_Data);
    // 制作QList<QList<QPointF>>数据列表
    QVector<QVector<QPointF>> get_QVector(const vector<vector<double>>&);
};
#endif // MAINWINDOW_H
