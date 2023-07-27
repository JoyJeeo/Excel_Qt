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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <actions\buttons\include\Button_All_Attri.h>
#include <actions\buttons\include\Button_Merge_File.h>

QT_BEGIN_NAMESPACE

class Ui_Main_Window
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    Button_All_Attri *btn_all_attri;
    Button_Merge_File *btn_merge;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *Main_Window)
    {
        if (Main_Window->objectName().isEmpty())
            Main_Window->setObjectName(QString::fromUtf8("Main_Window"));
        Main_Window->resize(811, 598);
        centralwidget = new QWidget(Main_Window);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(50, 30, 151, 81));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        btn_all_attri = new Button_All_Attri(verticalLayoutWidget);
        btn_all_attri->setObjectName(QString::fromUtf8("btn_all_attri"));

        verticalLayout->addWidget(btn_all_attri);

        btn_merge = new Button_Merge_File(verticalLayoutWidget);
        btn_merge->setObjectName(QString::fromUtf8("btn_merge"));

        verticalLayout->addWidget(btn_merge);

        Main_Window->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Main_Window);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Main_Window->setStatusBar(statusbar);
        menubar = new QMenuBar(Main_Window);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 811, 23));
        Main_Window->setMenuBar(menubar);

        retranslateUi(Main_Window);

        QMetaObject::connectSlotsByName(Main_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Main_Window)
    {
        Main_Window->setWindowTitle(QCoreApplication::translate("Main_Window", "MainWindow", nullptr));
        btn_all_attri->setText(QCoreApplication::translate("Main_Window", "btn_all_attri", nullptr));
        btn_merge->setText(QCoreApplication::translate("Main_Window", "btn_merge", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main_Window: public Ui_Main_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
