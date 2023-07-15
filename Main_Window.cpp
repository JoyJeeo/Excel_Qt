#include "Main_Window.h"
#include "ui_mainwindow.h"
#include "widgets/Widgets_Header_Proxy_01.h"
#include <QPushButton>

Main_Window::Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    try {
        resize(800,800);
        ui->setupUi(this);

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
        // 所有属性展示widget
        QPushButton* bt_all_attri_show = new QPushButton("Show all attributes",this);
        bt_all_attri_show->setFont(QFont("Consolas")); // 设置字体样式
        bt_all_attri_show->adjustSize(); // 按钮大小适应字体大小
        connect(bt_all_attri_show,&QPushButton::released,this,&Main_Window::task_widget_all_attri_show);

    } catch (...) {
        qDebug() << "Main_Window::total_task";
        throw;
    }
}

void Main_Window::task_widget_all_attri_show()
{
    /*执行显示窗体的任务*/
    try{
        Widget_All_Attri_Show* widget = nullptr;
        try {
            widget = new Widget_All_Attri_Show;
            widget->setAttribute(Qt::WA_DeleteOnClose); // 当窗体关闭时，回收内存
            widget->resize(800,800);
        } catch (...) {
            qDebug() << "widget->total_task()";
            throw;
        }
        bool flage = false;
        flage = widget->total_task();
        // 判断任务是否执行成功
        if(!flage){
            delete widget;
        }
        // 使用非模态窗口实现
        else widget->show();

    }
    catch(...){
        qDebug()<<"Main_Window::task_widget_all_attri_show()";
        throw;
    }
}



