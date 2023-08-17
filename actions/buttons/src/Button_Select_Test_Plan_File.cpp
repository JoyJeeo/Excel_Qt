#include "actions/buttons/include/Button_Select_Test_Plan_File.h"

#include <QDebug>
#include <QFileDialog>

extern QString test_plan_path; // 全局的test_plan文件的地址

Button_Select_Test_Plan_File::Button_Select_Test_Plan_File(QWidget *parent)
    :QPushButton(parent)
{

}

Button_Select_Test_Plan_File::~Button_Select_Test_Plan_File()
{

}

void Button_Select_Test_Plan_File::total_task()
{
    try {
        QFont font = QFont("Consolas");
        font.setPixelSize(18);
        this->setText(" Select Test Plan ");
        this->setFont(font);
        this->adjustSize();

        connect(this,&QPushButton::released,this,
                &Button_Select_Test_Plan_File::task_select_test_plan);

    } catch (...) {
        qDebug() << "Button_Select_Test_Plan_File::total_task";
        throw;
    }
}

void Button_Select_Test_Plan_File::task_select_test_plan()
{
    try {
        // 选择test plan文件
        QString path = QFileDialog::getOpenFileName(Q_NULLPTR,
                                             QObject::tr("select test plan file"),
                                             QObject::tr("../"), // 是指初始打开文件的位置
                                             QObject::tr("File(*.csv);;All(*.*)"));
        // 如果path不为空
        if(path.size() != 0)
        {
            test_plan_path = path;
        }
        // 否则什么也不做

    } catch (...) {
        qDebug() << "Button_Select_Test_Plan_File::task_select_test_plan";
        throw;
    }
}
