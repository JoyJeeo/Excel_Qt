#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "widgets/Widgets_Header_Proxy_01.h"
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFileDialog>
#include <QLabel>

Main_Window::Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Main_Window)
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
        // 所有属性展示widget【！！！】【优化封装按钮】
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
        // 多文件打开
        QStringList input_file_paths = QFileDialog::getOpenFileNames(Q_NULLPTR, // 不设置父窗体，独立窗体显示dialog即可
                                                       QObject::tr("select open files"), // 设置窗体标题
                                                       QObject::tr("../"), // 是指初始打开文件的位置
                                                       QObject::tr("File(*.csv);;All(*.*)")); // 设置可以筛选的文件类型
        // 如果没有任何打开的文件，则什么都不做
        if(input_file_paths.size() == 0)return;

        // 如果有选择需要打开的文件
        // 创建Widget_All_Attri_Show*容器，将所有显示结果打印在QScrollArea*中
        QVector<Widget_All_Attri_Show*> widgets;
        // 添加所属文件的名称
        QVector<QLabel*> file_paths;
        // 遍历所有需要打开的多文件，依次创建widget进行处理分析
        // 如果打开文件个数为0，则默认不会进入循环【直接推出此任务，无需手动处理】
        for(auto iterator = input_file_paths.begin();iterator != input_file_paths.end();iterator++)
        {
//          qDebug() << *iterator;
            // 依次创建draw窗体
            Widget_All_Attri_Show* widget = nullptr;
            try {
                widget = new Widget_All_Attri_Show;
//                widget->setMinimumSize(1200,1200);
//                widget->setMaximumSize(5000,5000);
    //            widget->setAttribute(Qt::WA_DeleteOnClose); // 当窗体关闭时，回收内存
    //            widget->resize(800,800);
    //            widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            } catch (...) {
                qDebug() << "Main_Window::task_widget_all_attri_show -> new Widget_All_Attri_Show";
                throw;
            }
            bool flage = false;
            // 将需要处理的文件路径，输入到程序中进行处理
            flage = widget->total_task(iterator->toStdString());
            // 判断任务是否执行成功
            if(!flage){
                delete widget;
            }
            // 使用非模态窗口实现
            else {
                // 将创建好的窗体添加到widgets中
                widgets.push_back(widget);
                // 将窗体的所属文件添加到容器中
                QLabel* label = new QLabel(QString::fromStdString(widget->get_window_title()));
                QFont font = QFont("Consolas");
                font.setStyleStrategy(QFont::PreferAntialias);
                font.setPointSize(14);
                font.setBold(true);
                label->setFont(font);
                QPalette pe;
                pe.setColor(QPalette::WindowText,QColor(91,155,213));
                label->setPalette(pe);
                file_paths.push_back(label);
    //            widget->show();
//                QScrollArea* scrollarea = new QScrollArea;
//                scrollarea->resize(1090,562);
//                scrollarea->setWindowTitle(QString::fromStdString(widget->get_window_title()));
//                scrollarea->setWidgetResizable(true);
//                scrollarea->setWidget(widget);
//                scrollarea->setAttribute(Qt::WA_DeleteOnClose);
//                scrollarea->show();
            }
        }
        // 创建QWidget容纳所有widget
        QWidget* main_widge = new QWidget;
        // 创建垂直窗体布局，将窗体们垂直分布
        QVBoxLayout* v_layout = new QVBoxLayout(main_widge);
//        v_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        main_widge->setLayout(v_layout);
        // 将所有widgets添加到QScrollArea中进行显示
        for(int i = 0;i < widgets.size();i++)
        {
            // 按照窗体文件位置 + 窗体内容的垂直分布结构进行添加入窗体
            v_layout->addWidget(file_paths[i]);
            v_layout->addWidget(widgets[i]);
        }
        // 抓取窗口生成图片并保存
        QPixmap pixMap_ = QPixmap::grabWidget(main_widge);
//        pixMap_ =
//                pixMap_.scaled(1082, 5000, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixMap_.save("./Widgets.png");

        // 构造QScrollArea
        QScrollArea* scrollarea = new QScrollArea;
        scrollarea->resize(1100,562);
        scrollarea->setWindowTitle("Volturrent: Chips Analysis");
        scrollarea->setWidgetResizable(true);
        scrollarea->setAttribute(Qt::WA_DeleteOnClose);
        scrollarea->setWidget(main_widge);
        scrollarea->show();



    }
    catch(...){
        qDebug()<<"Main_Window::task_widget_all_attri_show()";
        throw;
    }
}



