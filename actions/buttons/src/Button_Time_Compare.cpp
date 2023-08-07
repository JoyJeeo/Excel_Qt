#include "actions/buttons/include/Button_Time_Compare.h"

#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QDirIterator>

extern size_t pic_pages;

Button_Time_Compare::Button_Time_Compare(QWidget *parent)
    :QPushButton(parent)
{
    // 一次ui构造，total_task一次自己构造
}

Button_Time_Compare::~Button_Time_Compare()
{

}

void Button_Time_Compare::total_task()
{
    /*
        设置按钮样式，并进行信号关联
    */
    try {
        QFont font = QFont("Consolas");
//        font.setPixelSize(11);
        font.setPixelSize(18);
//        font.setBold(true);
        this->setText("Time file compare");
        this->setFont(font); // 设置字体样式
        this->adjustSize(); // 按钮大小适应字体大小

        connect(this,&QPushButton::released,this,&Button_Time_Compare::task_widget_time_file_comp_show);

    } catch (...) {
        qDebug() << "Button_Time_Compare::total_task";
        throw;
    }
}

void Button_Time_Compare::task_widget_time_file_comp_show()
{
    /*
        按钮的任务
        // 这里暂时先不管内存泄漏的问题
    */
    try{
        // 打开多文件
        input_dir_paths = construct_input_dir_paths();

        // 如果没有选择任何文件夹，则什么都不做
        if(input_dir_paths.size() == 0)return;
        // 如果有选择需要打开的文件
        construct_widgets_dir_paths();

        // 构造main_widget
        construct_main_widge();

        // 将widgets窗体内容进行保存
        save_pic(main_widge,"./TIME_COMPARE_PIC.png");

        // 构造QScrollArea
        construct_scrollarea();

        // 页数清空【保证多文件读取时，产生的依然是当前显示的widgets内容相同，而不是一个又一个，造成文件名重复而被覆盖】
        // 一个任务结束后，清空一次
        pic_pages = 1;
    }
    catch(...){
        qDebug()<<"Button_Time_Compare::task_widget_all_attri_show";
        throw;
    }

}

QStringList Button_Time_Compare::construct_input_dir_paths() noexcept
{
    /*
        功能：
            返回选中的文件夹，
                如果选中，则返回文件夹的路径集合
                如果没有选中，则返回空的容器size == 0
    */
    // 创建文件夹选择对话框
    QFileDialog fileDialog(nullptr, QString("Select Time Dirs"), QString("../"), QString("Dirs"));
    fileDialog.setFileMode(QFileDialog::Directory);
    // 其实还可以实现文件与文件夹的混选
    fileDialog.setOptions(QFileDialog::DontUseNativeDialog |
                          QFileDialog::ShowDirsOnly);

    QListView *listView = fileDialog.findChild<QListView*>("listView");
    if (NULL != listView)
    {
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    QTreeView *treeView = fileDialog.findChild<QTreeView*>();
    if (NULL != treeView)
    {
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    // 存储所有文件夹的路径的容器
    QStringList folders;
    // 执行dialog，如果执行成功，则填充文件夹路径容器
    if (fileDialog.exec() == QDialog::Accepted)
    {
        folders = fileDialog.selectedFiles();
//        qDebug() << folders;
    }

    // 无论容器中是否有文件夹路径，都返回
    return folders;
}

void Button_Time_Compare::construct_widgets_dir_paths()
{
    try {
        // 将容器先清空
        widgets.clear();file_paths.clear();

        // 遍历所有需要打开的多文件
        // 依次创建draw窗体
        Widget_All_Attri_Show* widget = nullptr;
        // 设置label字体
        QFont font = QFont("Consolas");
        font.setStyleStrategy(QFont::PreferAntialias);
        font.setPixelSize(20);
        font.setBold(true);
        QPalette pe; // 调色板
        pe.setColor(QPalette::WindowText,QColor(91,155,213));


        // 创建窗体
        widget = new Widget_All_Attri_Show;
        widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        // 判断任务是否执行成功
        bool flage = false;
        // 将需要处理的文件路径，输入到程序中进行处理
        flage = widget->time_task(input_dir_paths);
        // 任务成功
        // 使用非模态窗口实现
        if(flage){
            // 将创建好的窗体添加到widgets中
            widgets.push_back(widget);

            // 将窗体的所属文件添加到容器中
            QLabel* label = new QLabel(QString::fromStdString(widget->get_window_title()));
            // 设置label的美观性
            label->setFont(font);
            label->setPalette(pe);
            // 将label添加入labels
            file_paths.push_back(label);
        }
        // 如果任务执行失败
        else {
            // 将创建的窗体删除
            delete widget;
        }


    } catch (...) {
        qDebug() << "Button_Time_Compare::get_attri_widgets_filepath";
        throw ;
    }
}

void Button_Time_Compare::construct_main_widge()
{
    try {
        // 容纳所有widget
        main_widge = new QWidget;
        // 创建垂直窗体布局，将窗体们垂直分布
        QVBoxLayout* v_layout = new QVBoxLayout(main_widge);
        main_widge->setLayout(v_layout);
        // 将所有widgets添加到QScrollArea中进行显示
        for(int i = 0;i < widgets.size();i++)
        {
            // 按照窗体文件位置 + 窗体内容的垂直分布结构进行添加入窗体
            v_layout->addWidget(file_paths[i]);
            v_layout->addWidget(widgets[i]);
        }

    } catch (...) {
        qDebug() << "Button_Time_Compare::construct_main_widge";
        throw;
    }
}

void Button_Time_Compare::construct_scrollarea()
{
    try {
        // 构造QScrollArea
        scrollarea = new QScrollArea;
//        scrollarea->resize(1100,562);
        scrollarea->resize(800,620);
//        scrollarea->resize(595,842); // A4
        scrollarea->setWindowTitle("Volturrent: Chips Analysis");
        scrollarea->setWidgetResizable(true);
        scrollarea->setAttribute(Qt::WA_DeleteOnClose);
        scrollarea->setWidget(main_widge);
        scrollarea->show();

    } catch (...) {
        qDebug() << "Button_Time_Compare::construct_scrollarea";
        throw ;
    }
}

void Button_Time_Compare::save_pic(QWidget *saved_widget, const QString &saved_path)
{
    try {
        // 抓取窗口生成图片并保存
        QPixmap pixMap_ = QPixmap::grabWidget(saved_widget);
        pixMap_.save(saved_path);

    } catch (...) {
        qDebug() <<"Button_Time_Compare::save_pic";
        throw ;
    }
}


























