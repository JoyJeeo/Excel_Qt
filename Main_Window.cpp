#include "Main_Window.h"
#include "ui_Main_Window.h"
#include <QDebug>

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

    } catch (...) {
        qDebug() << "Main_Window::total_task";
        throw;
    }
}



