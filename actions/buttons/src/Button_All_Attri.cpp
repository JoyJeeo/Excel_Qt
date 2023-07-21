#include "../include/Button_All_Attri.h"

#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFileDialog>


Button_All_Attri::Button_All_Attri(QWidget *parent)
    :QPushButton(parent)
{
    // 一次ui构造，total_task一次自己构造
}

Button_All_Attri::~Button_All_Attri()
{

}

void Button_All_Attri::total_task()
{
    /*
        设置按钮样式，并进行信号关联
    */
    try {
        this->setText("Show all attributes");
        this->setFont(QFont("Consolas")); // 设置字体样式
        this->adjustSize(); // 按钮大小适应字体大小

        connect(this,&QPushButton::released,this,&Button_All_Attri::task_widget_all_attri_show);

    } catch (...) {
        qDebug() << "All_Attri_Button::total_task";
        throw;
    }
}

void Button_All_Attri::task_widget_all_attri_show()
{
    /*
        按钮的任务
        // 这里暂时先不管内存泄漏的问题
    */
    try{
        // 打开多文件
        input_file_paths = construct_input_file_paths();

        // 如果没有任何打开的文件，则什么都不做
        if(input_file_paths.size() == 0)return;
        // 如果有选择需要打开的文件
        construct_widgets_file_paths();

        // 构造main_widget
        construct_main_widge();

        // 将widgets窗体内容进行保存
        save_pic(main_widge,"./ALL_ATTRI_PIC.png");

        // 构造QScrollArea
        construct_scrollarea();

    }
    catch(...){
        qDebug()<<"All_Attri_Button::task_widget_all_attri_show";
        throw;
    }

}

QStringList Button_All_Attri::construct_input_file_paths() noexcept
{
    return QFileDialog::getOpenFileNames(Q_NULLPTR, // 不设置父窗体，独立窗体显示dialog即可
                                         QObject::tr("select open files"), // 设置窗体标题
                                         QObject::tr("../"), // 是指初始打开文件的位置
                                         QObject::tr("File(*.csv);;All(*.*)")); // 设置可以筛选的文件类型
}

void Button_All_Attri::construct_widgets_file_paths()
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
        font.setPointSize(14);
        font.setBold(true);
        QPalette pe; // 调色板
        pe.setColor(QPalette::WindowText,QColor(91,155,213));

        // 遍历所有文件路径
        for(auto iterator = input_file_paths.begin();iterator != input_file_paths.end();iterator++)
        {
            widget = new Widget_All_Attri_Show;
            widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

            // 判断任务是否执行成功
            bool flage = false;
            // 将需要处理的文件路径，输入到程序中进行处理
            flage = widget->total_task(iterator->toStdString());
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
        }

    } catch (...) {
        qDebug() << "All_Attri_Button::get_attri_widgets_filepath";
        throw ;
    }
}

void Button_All_Attri::construct_main_widge()
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
        qDebug() << "All_Attri_Button::construct_main_widge";
        throw;
    }
}

void Button_All_Attri::construct_scrollarea()
{
    try {
        // 构造QScrollArea
        scrollarea = new QScrollArea;
        scrollarea->resize(1100,562);
        scrollarea->setWindowTitle("Volturrent: Chips Analysis");
        scrollarea->setWidgetResizable(true);
        scrollarea->setAttribute(Qt::WA_DeleteOnClose);
        scrollarea->setWidget(main_widge);
        scrollarea->show();

    } catch (...) {
        qDebug() << "All_Attri_Button::construct_scrollarea";
        throw ;
    }
}

void Button_All_Attri::save_pic(QWidget *saved_widget, const QString &saved_path)
{
    try {
        // 抓取窗口生成图片并保存
        QPixmap pixMap_ = QPixmap::grabWidget(saved_widget);
        pixMap_.save(saved_path);

    } catch (...) {
        qDebug() <<"All_Attri_Button::save_pic";
        throw ;
    }
}


























