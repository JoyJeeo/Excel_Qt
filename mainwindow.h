#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chart.h"
#include "operate_file.h"

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

    Chart* initChart(const string& label,const vector<vector<double>>& data);
    void while_draw(const ifstream& ifs);
};
#endif // MAINWINDOW_H
