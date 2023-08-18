/********************************************************************************
** Form generated from reading UI file 'Main_Window.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <actions\buttons\include\Button_All_Attri.h>
#include <actions\buttons\include\Button_Merge_File.h>
#include <actions\buttons\include\Button_Select_Test_Plan_File.h>
#include <actions\buttons\include\Button_Temperature.h>
#include <actions\buttons\include\Button_Time_Compare.h>

QT_BEGIN_NAMESPACE

class Ui_Main_Window
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    Button_Temperature *btn_temperature;
    Button_All_Attri *btn_all_attri;
    Button_Merge_File *btn_merge;
    Button_Time_Compare *btn_time_compare;
    Button_Select_Test_Plan_File *btn_select_testplan;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *Main_Window)
    {
        if (Main_Window->objectName().isEmpty())
            Main_Window->setObjectName(QString::fromUtf8("Main_Window"));
        Main_Window->resize(811, 598);
        centralwidget = new QWidget(Main_Window);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 40, 345, 211));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetMinAndMaxSize);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        btn_temperature = new Button_Temperature(gridLayoutWidget);
        btn_temperature->setObjectName(QString::fromUtf8("btn_temperature"));

        gridLayout_2->addWidget(btn_temperature, 3, 0, 1, 1);

        btn_all_attri = new Button_All_Attri(gridLayoutWidget);
        btn_all_attri->setObjectName(QString::fromUtf8("btn_all_attri"));

        gridLayout_2->addWidget(btn_all_attri, 0, 0, 1, 1);

        btn_merge = new Button_Merge_File(gridLayoutWidget);
        btn_merge->setObjectName(QString::fromUtf8("btn_merge"));

        gridLayout_2->addWidget(btn_merge, 1, 0, 1, 1);

        btn_time_compare = new Button_Time_Compare(gridLayoutWidget);
        btn_time_compare->setObjectName(QString::fromUtf8("btn_time_compare"));

        gridLayout_2->addWidget(btn_time_compare, 2, 0, 1, 1);

        btn_select_testplan = new Button_Select_Test_Plan_File(gridLayoutWidget);
        btn_select_testplan->setObjectName(QString::fromUtf8("btn_select_testplan"));

        gridLayout_2->addWidget(btn_select_testplan, 0, 1, 1, 1);

        Main_Window->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Main_Window);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Main_Window->setStatusBar(statusbar);
        menubar = new QMenuBar(Main_Window);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 811, 30));
        Main_Window->setMenuBar(menubar);

        retranslateUi(Main_Window);

        QMetaObject::connectSlotsByName(Main_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Main_Window)
    {
        Main_Window->setWindowTitle(QCoreApplication::translate("Main_Window", "MainWindow", nullptr));
        btn_temperature->setText(QCoreApplication::translate("Main_Window", "btn_temperature", nullptr));
        btn_all_attri->setText(QCoreApplication::translate("Main_Window", "btn_all_attri", nullptr));
        btn_merge->setText(QCoreApplication::translate("Main_Window", "btn_merge", nullptr));
        btn_time_compare->setText(QCoreApplication::translate("Main_Window", "btn_time_compare", nullptr));
        btn_select_testplan->setText(QCoreApplication::translate("Main_Window", "btn_select_testplan", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main_Window: public Ui_Main_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
