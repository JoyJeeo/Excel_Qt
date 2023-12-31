 #include "Main_Window.h"
#include "ui_Main_Window.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

// 照片的页数
size_t pic_pages = 1;

QString test_plan_path = "";

void init_test_plan()
{
    /*
        只执行一次的函数
    */
//    static bool is_first_testplan = true;
//    if(is_first_testplan)
//    {
        // 初始化[默认]的test_plan的路径
//        test_plan_path =
//                QCoreApplication::applicationDirPath() + "/" +
//                "test_plan.csv";

        test_plan_path = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                       QObject::tr("select init test plan"),
                                                       QObject::tr("../"),
                                                       QObject::tr("File(*.csv);;All(*.*)"));
//        is_first_testplan = false;
//    }
}

Main_Window::Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Main_Window)
{
    try {
        ui->setupUi(this);
        resize(800,600);
        total_task();

    } catch (...) {
        qDebug() << "Main_Window::Main_Window";
        throw;
    }
}

Main_Window::~Main_Window()
{
    delete ui;
}

void Main_Window::total_task()
{
    try {
        ui->btn_all_attri->total_task();
        ui->btn_merge->total_task();
        ui->btn_time_compare->total_task();
        ui->btn_temperature->total_task();
        ui->btn_select_testplan->total_task();

        // 初始化test plan
        // 提示选择初始test plan
        QMessageBox::information(nullptr,"Init Test Plan",
                           "Select Init Test Plan");
        // 初始化test plan
        init_test_plan();

    } catch (...) {
        qDebug() << "Main_Window::total_task";
        throw;
    }
}



