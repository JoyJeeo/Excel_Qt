/********************************************************************************
** Form generated from reading UI file 'mainwindow01.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW01_H
#define UI_MAINWINDOW01_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow01
{
public:
    QWidget *centralwidget;
    QPushButton *pbt02;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow01)
    {
        if (MainWindow01->objectName().isEmpty())
            MainWindow01->setObjectName(QString::fromUtf8("MainWindow01"));
        MainWindow01->resize(800, 600);
        centralwidget = new QWidget(MainWindow01);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pbt02 = new QPushButton(centralwidget);
        pbt02->setObjectName(QString::fromUtf8("pbt02"));
        pbt02->setGeometry(QRect(570, 420, 161, 71));
        MainWindow01->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow01);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow01->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow01);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow01->setStatusBar(statusbar);

        retranslateUi(MainWindow01);

        QMetaObject::connectSlotsByName(MainWindow01);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow01)
    {
        MainWindow01->setWindowTitle(QCoreApplication::translate("MainWindow01", "MainWindow", nullptr));
        pbt02->setText(QCoreApplication::translate("MainWindow01", "\350\277\224\345\233\236\344\270\212\344\270\200\344\270\252\351\241\265\351\235\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow01: public Ui_MainWindow01 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW01_H
