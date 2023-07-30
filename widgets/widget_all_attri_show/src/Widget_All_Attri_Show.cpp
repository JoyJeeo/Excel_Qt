#include "widgets/widget_all_attri_show/include/Widget_All_Attri_Show.h"

extern size_t pic_pages;

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QSpacerItem>

const string Widget_All_Attri_Show::tackel_file_name()
{
    /*
        功能：
            将目标文件的绝对路径中分析获取目标文件的文件名并返回
    */

    try {
        size_t i;

        string in_file_path = src_file_manager->get_input_file_path();
//        cout << in_file_path << endl;

        for(i = in_file_path.size()-1;i >=0 ;i--)
        {
            if(in_file_path[i] == '/' || in_file_path[i] == '\\')break;
        }
        // 设置获取的文件名
        src_file_name = in_file_path.substr(i+1,in_file_path.size()-i+1);
//        qDebug() << src_file_name.c_str();
        return src_file_name;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::tackel_file_name";
        throw;
    }
}

//void Widget_All_Attri_Show::profile_scatter_sites()
//{
//    /*
//        获取scatter_sites，其是该类的一个属性，由private函数进行维护
//    */
//    try {
//        scatter_sites = datas->get_site_parts().get_Scatter_Site_Number();
////        qDebug() << scatter_sites.size();
////        exit(1);
//    } catch (...) {
//        qDebug() << "Widget_All_Attri_Show::profile_scatter_sites";
//        throw;
//    }
//}

//void Widget_All_Attri_Show::profile_site_max_parts()
//{
//    try {
//       site_max_parts = datas->get_site_parts().get_Max_Part_Id();
//    } catch (...) {
//        qDebug() << "Widget_All_Attri_Show::profile_site_max_parts";
//        throw ;
//    }
//}

//void Widget_All_Attri_Show::profile_site_list()
//{
//    try {
//        site_list = datas->get_site_parts().get_site_list();
//    } catch (...) {
//        qDebug() << "Widget_All_Attri_Show::profile_site_list";
//        throw;
//    }
//}

//void Widget_All_Attri_Show::profile_scatter_time_sites()
//{
//    try {
//        scatter_time_sites = datas->get_site_parts().get_Scatter_Time_Site_Number();

//    } catch (...) {
//        qDebug() << "Widget_All_Attri_Show::profile_scatter_time_sites";
//        throw;
//    }
//}

//void Widget_All_Attri_Show::profile_time_site_max_parts()
//{
//    try {
//        time_site_max_parts = datas->get_site_parts().get_Max_Time_Part_Id();
//    } catch (...) {
//        qDebug() << "Widget_All_Attri_Show::profile_time_site_max_parts";
//        throw;
//    }
//}

void Widget_All_Attri_Show::time_while_draw(int row_obj_nums)
{
    try {
        // 获取attri -> [site][part]的series数据,存于map<string,vector<vector<double>>>中
        map<string,map<string,vector<double>>> time_series_datas = datas->get_time_series_datas();
        map<string,map<string,vector<double>>> ration_time_series_datas =
                ration_datas->get_time_series_datas();

        // 获取target_file中有效的属性label
        auto labels = datas->get_time_labels();

        // 产生存储照片文件
        // 将文件夹下的内容清空
        QString pic_dir = build_pic_dir();
        // 如果不是第一页开始，则不清空文件内容
        if(pic_pages == 1) clear_pic_dir(pic_dir);

        // 循环存储图片，将图片进行存储
        // 图片存储容器
        QWidget* pic = new QWidget;
        QGridLayout* pic_layout = new QGridLayout(pic);
        pic_layout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 设置网格布局管理器的一格的最小和最大大小
        pic_layout->setHorizontalSpacing(0);
        pic_layout->setVerticalSpacing(0);

        // 纸张计算参数
        int pic_row_obj_nums = 2; // 图片中一行chart的个数[lie]
        int page_chart_nums = 3; // 一页显示多少行
        int page_charts = pic_row_obj_nums * page_chart_nums;

        // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
        // 将chart表格依次添加入主框体中，依次显示
        size_t i;
        for(i = 0;i < labels.size();) // labels.size()
        {
    //        auto x = series_datas.find(labels[i]); // x->first报错，不知道为什么
            string attri = labels[i];
            // 获取site_part的对应数据点
            QMap<string,QVector<QPointF>> time_site_points = get_time_matrix_pointF(time_series_datas[attri],datas->get_site_parts().get_Scatter_Time_Site_Number(),
                                                                                    datas->get_site_parts().get_Max_Time_Part_Id());
            QMap<string,QVector<QPointF>> ration_time_site_points =
                    get_time_matrix_pointF(ration_time_series_datas[attri],ration_datas->get_site_parts().get_Scatter_Time_Site_Number(),
                                           ration_datas->get_site_parts().get_Max_Time_Part_Id());

            // 【生成数据区chart】
            // 获取初始化好的chart
            Chart* chart = this->time_initChart(attri,time_site_points,datas,
                                                datas->get_site_parts().get_Scatter_Time_Site_Number(),
                                                datas->get_site_parts().get_Max_Time_Part_Id());
            // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
            this->pGridLayout->addWidget(chart,i/row_obj_nums+1,i%row_obj_nums+1);

            // 存储并保存
            // 填充pic 【会进行覆盖】
            Chart* t_chart = this->time_initChart(attri,time_site_points,datas,
                                                  datas->get_site_parts().get_Scatter_Time_Site_Number(),
                                                  datas->get_site_parts().get_Max_Time_Part_Id());
            int row = ((i/pic_row_obj_nums) % page_chart_nums + 1);
            int col = (i % pic_row_obj_nums + 1);
            pic_layout->addWidget(t_chart,row,col);

            // 6个chart一存储
            if((i+1) % page_charts == 0)
            {
                QString path = pic_dir + "/PIC_" +
                        // 计算当前是第几页
                        // i / page_charts + 1
                        QString::fromStdString(to_string(pic_pages))+
                        QString(".png");
//                qDebug() << path;
                save_pic(pic,path);

                // 页数++
                pic_pages++;

                // 生成其图片后，将布局内widget清空
                delete pic_layout;
                delete pic;
                // 重新初始化pic、pic_layout
                pic = new QWidget;
                pic_layout = new QGridLayout(pic);
                pic_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
                pic_layout->setHorizontalSpacing(0);
                pic_layout->setVerticalSpacing(0);
            }
            // 切换i
            i++;

            Chart* ration_chart = this->time_initChart(attri,ration_time_site_points,ration_datas,
                                                       ration_datas->get_site_parts().get_Scatter_Time_Site_Number(),
                                                       ration_datas->get_site_parts().get_Max_Time_Part_Id());
            // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
            this->pGridLayout->addWidget(ration_chart,i/row_obj_nums+1,i%row_obj_nums+1);

            t_chart = this->time_initChart(attri,ration_time_site_points,ration_datas,
                                           ration_datas->get_site_parts().get_Scatter_Time_Site_Number(),
                                           ration_datas->get_site_parts().get_Max_Time_Part_Id());;
            row = ((i/pic_row_obj_nums) % page_chart_nums + 1);
            col = (i % pic_row_obj_nums + 1);
            pic_layout->addWidget(t_chart,row,col);

            // 6个chart一存储
            if((i+1) % page_charts == 0)
            {
                QString path = pic_dir + "/PIC_" +
                        // 计算当前是第几页
                        // i / page_charts + 1
                        QString::fromStdString(to_string(pic_pages))+
                        QString(".png");
//                qDebug() << path;
                save_pic(pic,path);

                // 页数++
                pic_pages++;

                // 生成其图片后，将布局内widget清空
                delete pic_layout;
                delete pic;
                // 重新初始化pic、pic_layout
                pic = new QWidget;
                pic_layout = new QGridLayout(pic);
                pic_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
                pic_layout->setHorizontalSpacing(0);
                pic_layout->setVerticalSpacing(0);
            }
            // [!!!]
            i++;

        }
//        qDebug() << "i: " << i;
        // 最后一页是否被存储
        if(i % page_charts != 0) // i出来时已经i++了，这里不需要i+1
        {
            // 没有存储，则将最有一页存储
            QString path = pic_dir + "/PIC_" +
                    // 将余页获取，并打印
                    //i / page_charts + 1
                    QString::fromStdString(to_string(pic_pages)) +
                    QString(".png");
//            qDebug() << "Into";
//            qDebug() << path;
            save_pic(pic,path);

            // 页数++
            pic_pages++;

        }
//        pic->show();

        // 回收临时堆区内存
        delete pic_layout;
        delete pic;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::time_while_draw";
        throw;
    }
}

QMap<string,QVector<QPointF>> Widget_All_Attri_Show::get_time_matrix_pointF(map<string,vector<double>> &time_datas,
                                                                            const vector<string>& scatter_time_sites,
                                                                            int time_site_max_parts)
{
    try {
        // 返回值更换类型
        // 获取sites
        QMap<string,QVector<QPointF>> ans;
        // 初始化ans
        for(auto time_site : scatter_time_sites)
        {
            // 初始化所有数据为(0,0)作为数据为NULL时的断点标志
            ans.insert(time_site,QVector<QPointF>(time_site_max_parts+1,QPointF(0,0)));
        }

        // 将原本site_part_vals的数据:
        //      attri -> [site][part] = val ----> attri -> [site][part] = (part,val)
        // 只是将内部值转换为QPointF(列,数值) 并将 vector转换为QVector
        // 遍历site_part_vals
        for(size_t i = 0;i < scatter_time_sites.size();i++)
        {
            string time_site = scatter_time_sites[i];
            // 初始化所有数据为(0,0)作为数据为NULL时的断点标志
//            ans.insert(site,QVector<QPointF>(site_max_parts+1,QPointF(0,0))); // 由数据的结构可知，过程中插入和直接初始化两者并不等价

            // 遍历所有parts
            for(int part = 1;part <= time_site_max_parts;part++)

            {
                // 如果为NULL值,则用(0,0)作为断点标志
                if(time_datas[time_site][part] == NULL_Number) continue;
                // 如果正常数值，则存储将 val -> (part,val)
                ans[time_site][part] = QPointF(part,time_datas[time_site][part]);
            }
        }

//        test_for(ans);

        return ans;
    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::get_time_matrix_pointF";
        throw;
    }
}

Chart *Widget_All_Attri_Show::time_initChart(const string &attri,
                                             const QMap<string, QVector<QPointF>> &time_points,
                                             Targetfile_Valid_Data* Datas,
                                             const vector<string>& scatter_time_sites,
                                             int time_site_max_parts,
                                             double axisX_k, double axisY_k
                                             )
{

    try {
        //设置表头【attri的名称已经在这里设置给chart作为表名了】
        Chart* chart = new Chart(this,attri.c_str()); // 局部对象

        //设置坐标系
        // 设置X轴数据
        // 获取site_part
        auto site_part = Datas->get_site_parts(); // 【！！！】【这里设计横坐标的解耦，之后处理】

        // 设置Y轴数据
        // 获取属性单位
        QString unit = profile_time_attri_unit(attri,Datas);

        // 获取的数值的最值线的数值结果【这里的最值线，实际并不是最值线，可以理解为是为了让Y更合理而配合最值权衡使用的中间值】【！！！之后需要进行进一步维护】
        auto XI_line_data = profile_time_data_series_XI(attri,Datas);

        // 对纵坐标的大小范围进行处理【而不是简单的通过get_ul_compare_attri_XI函数获取的最值】
        // 获取倍距
        double zoom_dist = profile_time_zoom_dist_XI(XI_line_data);
        // 获取理论上初始纵坐标的的数值
        auto Y_XI = profile_time_Y_XI(XI_line_data,zoom_dist,axisY_k);

        // 综合处理最值线和理论纵坐标，使得初始数据图更[美观]
        auto realY_XI = profile_time_generalY_XI(XI_line_data,Y_XI);

        // 应该使用最大最小值之间的倍距来作为放大和缩小的依据【而不应该是线本身的数值】
        // 【灵活设置图像的放大缩小】
        // 设置坐标系的数值范围
        chart->setAxis(
                    // 横坐标
                    "PART_ID",1,time_site_max_parts,time_site_max_parts,
                    // 纵坐标
                    unit,realY_XI.first,realY_XI.second,
                    // 纵坐标的分割线的条数
                    12
                    );

        //绘制【注入数据点数值和最值】
        // 获取属性最值
        auto attri_uul = Datas->get_time_attri_uuls();
        auto attri_XI = make_pair(attri_uul.m_attri_uuls[attri].m_LimitL,
                                  attri_uul.m_attri_uuls[attri].m_LimitU);
        // 这里传入XI_line_data，为了判断是否需要画图；传入attri_XI，才是真正的最值线的数据【最值有数值就画，没有就不画】
        chart->time_buildChart(scatter_time_sites,time_site_max_parts,time_points,XI_line_data,attri_XI);

        return chart;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::time_initChart";
        throw;
    }
}

QString Widget_All_Attri_Show::profile_time_attri_unit(const string &attri,
                                                       Targetfile_Valid_Data* Datas)
{
    try {
        auto attri_uul = Datas->get_attri_uuls();
        // 获取属性的单位
        string unit = attri_uul.m_attri_uuls[attri].m_Unit;

        // 转换为QString
        return QString::fromStdString(unit);

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_time_attri_unit";
        throw;
    }
}

pair<double, double> Widget_All_Attri_Show::profile_time_data_series_XI(const string &attri,
                                                                        Targetfile_Valid_Data* Datas,
                                                                        double zoom)
{
    try {
        // 获取attri_uul
        auto attri_uul = Datas->get_time_attri_uuls();

        //设置坐标系
        // 获取最值
        // 获取属性最值
        auto attri_XI = make_pair(attri_uul.m_attri_uuls[attri].m_LimitL,
                                  attri_uul.m_attri_uuls[attri].m_LimitU);
        // 获取所有属性值的最值
        auto all_attri_XI = Datas->get_time_attri_XI(attri); // 【修改数据获取方式】

        // 经过attri_XI和all_attri_XI对比后，综合获取的数值的最值线的数值结果
        auto XI_line_data = attri_XI;

        // 构造XI_line_data // 【使得只要有属性数值，就一定可以画出最值线，没有属性数值则不画线】【完备】
        // XI_line_data: [INT_MIN,INT_MAX]、[正常数值范围]
        /* 更新属性最值：只有两种重要情况
         *      1. 当属性的最值不存在，属性数值的最值存在时，则使用属性数值的最值的0.05倍来替代
         *      2. 当属性的最值存在，而数值的最值不存在时，则最值线无意义【什么都不用画】
         *
         * 这里使用attri_XI作为判断最值线的主要条件，由其需求所导致
        */
        // 最小值构造
        // 如果最值有数据，但是数据没有数据，那么就不画最值线
        if(attri_XI.first != INT_MIN && all_attri_XI.first == INT_MIN)
        {
            XI_line_data.first = all_attri_XI.first;
        }
        // 如果数据有数据，但是最值没数据，那就不画最值线
        if(attri_XI.first == INT_MIN && all_attri_XI.first != INT_MIN)
        {
            // 对线进行倍数移动
            XI_line_data.first = all_attri_XI.first - abs(all_attri_XI.first) * zoom;
        }
        // 最大值构造
        if(attri_XI.second != INT_MAX && all_attri_XI.second == INT_MAX)
        {
            XI_line_data.second = all_attri_XI.second;
        }
        if(attri_XI.second == INT_MAX && all_attri_XI.second != INT_MAX)
        {
            // 对线进行倍数移动
            XI_line_data.second = all_attri_XI.second + abs(all_attri_XI.second) * zoom;
        }

        return XI_line_data;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_data_time_series_XI";
        throw;
    }
}

double Widget_All_Attri_Show::profile_time_zoom_dist_XI(const pair<double, double> &time_data_XI)
{
    try {
        return profile_zoom_dist_XI(time_data_XI);

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_time_zoom_dist_XI";
        throw;
    }
}

pair<double, double> Widget_All_Attri_Show::profile_time_Y_XI(const pair<double, double> &time_data_series_XI,
                                                              double zoom_dist, double axisY_k)
{
    try {
        return profile_Y_XI(time_data_series_XI,zoom_dist,axisY_k);
    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_time_Y_XI";
        throw;
    }
}

pair<double, double> Widget_All_Attri_Show::profile_time_generalY_XI(const pair<double, double> &time_data_series_XI,
                                                                     const pair<double, double> &Y_XI)
{
    try {
        return profile_generalY_XI(time_data_series_XI,Y_XI);
    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_time_generalY_XI";
        throw;
    }
}

const QString Widget_All_Attri_Show::build_pic_dir()
{
    /*
        功能：
            将项目exe所在的路径下，存在PICS文件夹
    */
    try {
        QString pro_path = QCoreApplication::applicationDirPath();
        QString ans = pro_path + "/" + PIC_FILE_NAME;

        QDir pic_dir(ans);
        // 判断文件是否已经存在
        // 如果文件夹已经存在
        if(!pic_dir.exists())
        {
            pic_dir.mkdir(ans);
        }

        return ans;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::build_pic_dir";
        throw;
    }
}

void Widget_All_Attri_Show::clear_pic_dir(const QString &pic_dir)
{
    /*
        功能：
            只移除文件夹下的所有内容
    */
    try {
        QDir dir(pic_dir);
        if(dir.isEmpty()) return;

        // 清空目录下的所有内容
        // 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
        // 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
        QDirIterator DirsIterator(pic_dir, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while(DirsIterator.hasNext())
        {
            if (!dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
            {
                QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
            }
        }


    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::clear_pic_dir";
        throw;
    }
}

void Widget_All_Attri_Show::test_for(QMap<int,QVector<QPointF>> &datas,
                                     const vector<int>& scatter_sites,
                                     int site_max_parts)
{
    for(size_t i = 0;i < scatter_sites.size();i++)
    {
        int site = scatter_sites[i];

        // 遍历所有parts
        for(int part = 1;part <= site_max_parts;part++)
        {
            qDebug() << datas[site][part];
        }
    }
}

const string Widget_All_Attri_Show::get_window_title() noexcept
{
    // 由于不同文件夹中会有同名文件，因此使用绝对路径的方式加以文件区分
    return "Analysis file：" + src_file_manager->get_input_file_path();
}

void Widget_All_Attri_Show::set_src_file_name(const string &str) noexcept
{
    src_file_name = str;
}

const string Widget_All_Attri_Show::get_src_file_name() noexcept
{
    return src_file_name;
}

Widget_All_Attri_Show::Widget_All_Attri_Show(QWidget *parent)
    : QWidget(parent)
{
    /*构造类内对象*/
    try {
        // 构造函数中只做一次性构造对象的
        resize(800,800);
        src_file_manager = new File_To_Targetfile;
        datas = new Targetfile_Valid_Data;
        ration_datas = new Targetfile_Valid_Data;
        this->pGridLayout = new QGridLayout(this); // 添加布局到主窗体中
        this->pGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 设置网格布局管理器的一格的最小和最大大小
        this->pGridLayout->setHorizontalSpacing(0);
        this->pGridLayout->setVerticalSpacing(0);
//        this->pGridLayout->setSpacing(0);

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::Widget_All_Attri_Show";
        throw;
    }
}

Widget_All_Attri_Show::~Widget_All_Attri_Show()
{

}

// 制作QList<QList<QPointF>>数据列表
QMap<int,QVector<QPointF>> Widget_All_Attri_Show::get_matrix_pointF(map<int,vector<double>>& site_part_vals,
                                                                    const vector<int>& scatter_sites,
                                                                    int site_max_parts)
{
    /*
       功能：
           转换site_part_vals中的数据和数据类型为QVector<QVector<QPointF>>进行传出,
            为series提供数据

        存储内容：
            attri -> [site][part] = val ----> attri -> [site][part] = (part,val)
                1. 当数据正常时，存储正常的点数据
                2. 当数据为NULL_Number的NULL时，存储(0,0)作为标志
    */
    try {
        // 返回值更换类型
        // 获取sites
        QMap<int,QVector<QPointF>> ans;
        // 初始化ans
        for(auto site : scatter_sites)
        {
            // 初始化所有数据为(0,0)作为数据为NULL时的断点标志
            ans.insert(site,QVector<QPointF>(site_max_parts+1,QPointF(0,0)));
        }

        // 将原本site_part_vals的数据:
        //      attri -> [site][part] = val ----> attri -> [site][part] = (part,val)
        // 只是将内部值转换为QPointF(列,数值) 并将 vector转换为QVector
        // 遍历site_part_vals
        for(size_t i = 0;i < scatter_sites.size();i++)
        {
            int site = scatter_sites[i];
            // 初始化所有数据为(0,0)作为数据为NULL时的断点标志
//            ans.insert(site,QVector<QPointF>(site_max_parts+1,QPointF(0,0))); // 由数据的结构可知，过程中插入和直接初始化两者并不等价

            // 遍历所有parts
            for(int part = 1;part <= site_max_parts;part++)
            {
                // 如果为NULL值,则用(0,0)作为断点标志
                if(site_part_vals[site][part] == NULL_Number) continue;
                // 如果正常数值，则存储将 val -> (part,val)
                ans[site][part] = QPointF(part,site_part_vals[site][part]);
            }
        }

//        test_for(ans);

        return ans;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::get_matrix_pointF";
        throw;
    }
}

//初始化图表
Chart* Widget_All_Attri_Show::initChart(const string& attri,
                             const QMap<int,QVector<QPointF>>& site_points,
                            const vector<int>& scatter_sites,
                            int site_max_parts,
                             double axisX_k,
                             double axisY_k
                            )
{
    /*
        这里设置y轴的放大倍数默认为1.2,留一个接口,为放大镜功能做准备
            [图像的方法/缩小,本质就是数轴的放大和缩小]

        属性最值线与纵坐标的放缩之间，没有直接关系【互相独立】
    */
    try {
        //设置表头【attri的名称已经在这里设置给chart作为表名了】
        Chart* chart = new Chart(this,attri.c_str()); // 局部对象

        //设置坐标系
        // 设置X轴数据
        // 获取site_part
        auto site_part = datas->get_site_parts(); // 【！！！】【这里设计横坐标的解耦，之后处理】

        // 设置Y轴数据
        // 获取属性单位
        QString unit = profile_attri_unit(attri);

        // 获取的数值的最值线的数值结果【这里的最值线，实际并不是最值线，可以理解为是为了让Y更合理而配合最值权衡使用的中间值】【！！！之后需要进行进一步维护】
        auto XI_line_data = profile_data_series_XI(attri);

        // 对纵坐标的大小范围进行处理【而不是简单的通过get_ul_compare_attri_XI函数获取的最值】
        // 获取倍距
        double zoom_dist = profile_zoom_dist_XI(XI_line_data);
        // 获取理论上初始纵坐标的的数值
        auto Y_XI = profile_Y_XI(XI_line_data,zoom_dist,axisY_k);

        // 综合处理最值线和理论纵坐标，使得初始数据图更[美观]
        auto realY_XI = profile_generalY_XI(XI_line_data,Y_XI);

        // 应该使用最大最小值之间的倍距来作为放大和缩小的依据【而不应该是线本身的数值】
        // 【灵活设置图像的放大缩小】
        // 设置坐标系的数值范围
        chart->setAxis(
                    // 横坐标
                    "PART_ID",1,site_max_parts,site_max_parts ,
                    // 纵坐标
                    unit,realY_XI.first,realY_XI.second,
                    // 纵坐标的分割线的条数
                    12
                    );

        //绘制【注入数据点数值和最值】
        // 获取属性最值
        auto attri_uul = datas->get_attri_uuls();
        auto attri_XI = make_pair(attri_uul.m_attri_uuls[attri].m_LimitL,
                                  attri_uul.m_attri_uuls[attri].m_LimitU);
        // 这里传入XI_line_data，为了判断是否需要画图；传入attri_XI，才是真正的最值线的数据【最值有数值就画，没有就不画】
        chart->buildChart(scatter_sites,site_max_parts,site_points,XI_line_data,attri_XI);

        return chart;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::initChart";
        throw;
    }
}

void Widget_All_Attri_Show::save_pic(QWidget *saved_widget, const QString &saved_path)
{
    try {
        // 抓取窗口生成图片并保存
        QPixmap pixMap_ = QPixmap::grabWidget(saved_widget);
        pixMap_.save(saved_path);

    } catch (...) {
        qDebug() <<"Widget_All_Attri_Show::save_pic";
        throw ;
    }
}

QString Widget_All_Attri_Show::profile_attri_unit(const string& attri)
{
    /*
        参数：
            attri：根据输入的属性，获取其对应的属性单位
        功能：
            将attri的属性单位获取，并转换为QString返回
    */
    try {
        // 获取attri_uul
        auto attri_uul = datas->get_attri_uuls();
        // 获取属性的单位
        string unit = attri_uul.m_attri_uuls[attri].m_Unit;

        // 转换为QString
        return QString::fromStdString(unit);

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_attri_unit";
        throw;
    }
}

pair<double, double> Widget_All_Attri_Show::profile_data_series_XI(const string &attri,double zoom)
{
    /*
        参数：
            attri：对应属性获取数据最值
            zoom：设置当最值不存在时，设置属性数值最值的放大倍数
        功能：
            经过attri_XI和all_attri_XI对比后，综合获取的数值的最值线的数值结果
            【获取属性对应数值的合理的最值线】
        数值范围：
            [INT_MIN,INT_MAX]、[正常数值范围]
    */
    try {
        // 获取attri_uul
        auto attri_uul = datas->get_attri_uuls();

        //设置坐标系
        // 获取最值
        // 获取属性最值
        auto attri_XI = make_pair(attri_uul.m_attri_uuls[attri].m_LimitL,
                                  attri_uul.m_attri_uuls[attri].m_LimitU);
        // 获取所有属性值的最值
        auto all_attri_XI = datas->get_attri_XI(attri);

        // 经过attri_XI和all_attri_XI对比后，综合获取的数值的最值线的数值结果
        auto XI_line_data = attri_XI;

        // 构造XI_line_data // 【使得只要有属性数值，就一定可以画出最值线，没有属性数值则不画线】【完备】
        // XI_line_data: [INT_MIN,INT_MAX]、[正常数值范围]
        /* 更新属性最值：只有两种重要情况
         *      1. 当属性的最值不存在，属性数值的最值存在时，则使用属性数值的最值的0.05倍来替代
         *      2. 当属性的最值存在，而数值的最值不存在时，则最值线无意义【什么都不用画】
         *
         * 这里使用attri_XI作为判断最值线的主要条件，由其需求所导致
        */
        // 最小值构造
        // 如果最值有数据，但是数据没有数据，那么就不画最值线
        if(attri_XI.first != INT_MIN && all_attri_XI.first == INT_MIN)
        {
            XI_line_data.first = all_attri_XI.first;
        }
        // 如果数据有数据，但是最值没数据，那就不画最值线
        if(attri_XI.first == INT_MIN && all_attri_XI.first != INT_MIN)
        {
            // 对线进行倍数移动
            XI_line_data.first = all_attri_XI.first - abs(all_attri_XI.first) * zoom;
        }
        // 最大值构造
        if(attri_XI.second != INT_MAX && all_attri_XI.second == INT_MAX)
        {
            XI_line_data.second = all_attri_XI.second;
        }
        if(attri_XI.second == INT_MAX && all_attri_XI.second != INT_MAX)
        {
            // 对线进行倍数移动
            XI_line_data.second = all_attri_XI.second + abs(all_attri_XI.second) * zoom;
        }

        return XI_line_data;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_data_series_XI";
        throw;
    }
}

pair<double, double> Widget_All_Attri_Show::profile_Y_XI(const pair<double, double> &data_series_XI,
                                                         double zoom_dist, double axisY_k)
{
    /*
        函数功能：
            获取纵坐标数值的理论范围
        数值范围：
            [-1.10,1.10]、[最小值线的放缩后的结果，最大值线的放缩后的结果]
    */
    try {
        // 获取理论上初始纵坐标在缩放后的正常数值【理论上，纵坐标的数值与数值的最值线的数值之间是相互独立的关系】
        // 设置理论缩放大小
        // 理论纵坐标的数值范围[-1.1,1.1]、[最小值线的放缩后的结果，最大值线的放缩后的结果]【无论如何，纵坐标都要有数值的】
        pair<double, double> Y_XI;
        /* data_series_XI.first/second代表最值线的数值，zoom_dist一般代表最值线之间距离的一半，
             axisY_k > 0代表缩小的倍数【纵坐标数值放大，则图像缩小】
             axisY_k < 0代表放大的倍数【纵坐标数值放小，则图像变大】【反比关系】
        */
        // 放小
        Y_XI.first = data_series_XI.first == INT_MIN ? -1.10 :
                                        data_series_XI.first - zoom_dist * axisY_k;
        // 放大
        Y_XI.second = data_series_XI.second == INT_MAX ? 1.10 :
                                        data_series_XI.second + zoom_dist * axisY_k;

        return Y_XI;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_Y_XI";
        throw;
    }
}

pair<double, double> Widget_All_Attri_Show::profile_generalY_XI(const pair<double, double> &data_series_XI,
                                                                const pair<double,double>& Y_XI)
{
    /*
        参数：
            data_series_XI： 数据线的最值线的数据
            Y_XI：纵坐标最值的数据
        功能：
            理论纵坐标的数值范围与实际最值线的数值范围，综合后，获得纵坐标的实际数值范围
            【使其能够包裹最值线的最大最小值】
            【综合处理，主要为了初始构图时的美观】
    */
    try {
        // 【初始图片美观 -> 缩放交给chartview】
        // 理论纵坐标的数值范围与实际最值线的数值范围，综合后，获得纵坐标的实际数值范围【使其能够包裹最值线的最大最小值】
        // 这样设置，会导致缩放图像无法缩放进极限最大值内部(除非不用他经过缩放)【除非，chartview可以实现缩放】
        // 【这里综合处理，主要为了初始构图时的美观】
        auto realY_XI = Y_XI; // 初始化Y_XI，防止无法进入if语句(数据全空的情况)
        if(data_series_XI.first != INT_MIN && data_series_XI.second != INT_MAX)
        {
            // 获取综合最值后，需要进一步放缩【防止重合线】【简单放缩】
            realY_XI.first = min(realY_XI.first,data_series_XI.first);
            realY_XI.second = max(realY_XI.second,data_series_XI.second);

            // 获取放缩倍距
            double inner_multi_dst = profile_zoom_dist_XI(realY_XI);

            // 进一步缩放
            realY_XI.first -= inner_multi_dst * 0.1;
            realY_XI.second += inner_multi_dst * 0.1;
        }

        return realY_XI;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_generalY_XI";
        throw;
    }
}

double Widget_All_Attri_Show::profile_zoom_dist_XI(const pair<double, double> &data_XI)
{
    /*
        参数：
            data_XI：数据上线限之间的最值
        功能：
            获取上下限之间距离的一半进行返回，
            如果上下限数值相等，则以其中一个数值的绝对值/2作为上下限距离的一半，进行返回
    */
    try {
        // 获取最值线之间的距离差
        double gap = 0.0;
        // 上下限距离的一半，为放缩距离
        double zoom_dist = 0.0;
        // 防止无最值时，数值错误
        if(data_XI.first != INT_MIN && data_XI.second != INT_MAX)
        {
            // 获取attri_XI最值之间的距离【正值】
            gap = abs(data_XI.second - data_XI.first);
            // 如果最值线之间存在gap，则倍距为
            if(gap != 0.0)
            {
                // 设置图像的倍距
                zoom_dist = gap / 2;
            }
            // 否则就设置为其中一个数值【按线进行放缩即可】
            else {
                zoom_dist = abs(data_XI.first/2);
            }
        }
        return zoom_dist;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_zoom_dist_XI";
        throw;
    }
}

void Widget_All_Attri_Show::while_draw(int row_obj_nums)
{
    /*
        功能：
            1. 循环添加所有生成的chart到网格布局管理器中
            2. 使用默认参数row_obj_nums的方式，来默认设置网格管理器一行的widget个数
    */

    try {
        // 获取attri -> [site][part]的series数据,存于map<string,vector<vector<double>>>中
        map<string,map<int,vector<double>>> series_datas = datas->get_series_datas();

        // 获取target_file中有效的属性label
        auto labels = datas->get_labels();

        // 产生存储照片文件
        // 将文件夹下的内容清空
        QString pic_dir = build_pic_dir();
        // 如果不是第一页开始，则不清空文件内容
        if(pic_pages == 1) clear_pic_dir(pic_dir);

        // 循环存储图片，将图片进行存储
        // 图片存储容器
        QWidget* pic = new QWidget;
        QGridLayout* pic_layout = new QGridLayout(pic);
        pic_layout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 设置网格布局管理器的一格的最小和最大大小
        pic_layout->setHorizontalSpacing(0);
        pic_layout->setVerticalSpacing(0);

        // 纸张计算参数
        int pic_row_obj_nums = 2; // 图片中一行chart的个数[lie]
        int page_chart_nums = 3; // 一页显示多少行
        int page_charts = pic_row_obj_nums * page_chart_nums;

        // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
        // 将chart表格依次添加入主框体中，依次显示
        size_t i;
        for(i = 0;i < labels.size();i++) // labels.size()
        {
    //        auto x = series_datas.find(labels[i]); // x->first报错，不知道为什么
            string attri = labels[i];
            // 获取site_part的对应数据点
            QMap<int,QVector<QPointF>> site_points = get_matrix_pointF(series_datas[attri],datas->get_site_parts().get_Scatter_Site_Number(),
                                                                       datas->get_site_parts().get_Max_Part_Id());

            // 获取初始化好的chart
            Chart* chart = this->initChart(attri,site_points,datas->get_site_parts().get_Scatter_Site_Number(),
                                           datas->get_site_parts().get_Max_Part_Id());
            // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
            this->pGridLayout->addWidget(chart,i/row_obj_nums+1,i%row_obj_nums+1);

            // 存储并保存
            // 填充pic 【会进行覆盖】
            Chart* t_chart = this->initChart(attri,site_points,datas->get_site_parts().get_Scatter_Site_Number(),
                                             datas->get_site_parts().get_Max_Part_Id());
            int row = ((i/pic_row_obj_nums) % page_chart_nums + 1);
            int col = (i % pic_row_obj_nums + 1);
            pic_layout->addWidget(t_chart,row,col);

            // 6个chart一存储
            if((i+1) % page_charts == 0)
            {
                QString path = pic_dir + "/PIC_" +
                        // 计算当前是第几页
                        // i / page_charts + 1
                        QString::fromStdString(to_string(pic_pages))+
                        QString(".png");
//                qDebug() << path;
                save_pic(pic,path);

                // 页数++
                pic_pages++;

                // 生成其图片后，将布局内widget清空
                delete pic_layout;
                delete pic;
                // 重新初始化pic、pic_layout
                pic = new QWidget;
                pic_layout = new QGridLayout(pic);
                pic_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
                pic_layout->setHorizontalSpacing(0);
                pic_layout->setVerticalSpacing(0);
            }
        }
//        qDebug() << "i: " << i;
        // 最后一页是否被存储
        if(i % page_charts != 0) // i出来时已经i++了，这里不需要i+1
        {
            // 没有存储，则将最有一页存储
            QString path = pic_dir + "/PIC_" +
                    // 将余页获取，并打印
                    //i / page_charts + 1
                    QString::fromStdString(to_string(pic_pages)) +
                    QString(".png");
//            qDebug() << "Into";
//            qDebug() << path;
            save_pic(pic,path);

            // 页数++
            pic_pages++;

        }
//        pic->show();

        // 回收临时堆区内存
        delete pic_layout;
        delete pic;


    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::while_draw";
        throw;
    }

}

bool Widget_All_Attri_Show::total_task(const string& input_file_path)
{
    /*使用中间文件，将目标文件的数据打开后，对其中内容进行绘制*/
    // 数据处理
    try{
        // 对原始数据进行处理和分析source_file -> target_file，获取目标文件的绝对路径
        // 【传入输入文件的路径，传出输出文件的路径】
        const string target_file_path = src_file_manager->total_task(input_file_path);

        // 打开目标文件后，输入的文件名才会被修改
        // 设置Widget的窗口名称
        this->setWindowTitle(get_window_title().c_str());

        // 输入文件路径获取成功
        ifstream ifs = src_file_manager->input_file_open(target_file_path);
        // 解析生成的target_file数据读入程序中
        datas->total_task(ifs);
        // 初始化scatter_sites,site_max_parts
//        profile_scatter_sites();
//        profile_site_list();
//        profile_site_max_parts();


        // 开始对数据循环扫描进行绘画
        while_draw();

        // 关闭打开的文件
        ifs.close();

        // 返回true代表任务执行成功
        return true;

    } catch(...){
        qDebug() <<"Widget_All_Attri_Show::total_task()";
        throw;
    }
}

bool Widget_All_Attri_Show::merge_task(const QStringList &files_path)
{
    try {
        // 【与total_task相比，只做了读取原数据时的数据分析操作】
        const string target_file_path = src_file_manager->merge_task(files_path);
        this->setWindowTitle(get_window_title().c_str());

        // 输入文件路径获取成功
        ifstream ifs = src_file_manager->input_file_open(target_file_path);
        // 解析生成的target_file数据读入程序中
        datas->total_task(ifs);
        // 初始化scatter_sites,site_max_parts
//        profile_scatter_sites();
//        profile_site_list();
//        profile_site_max_parts();

        // 开始对数据循环扫描进行绘画
        while_draw();

        // 关闭打开的文件
        ifs.close();

        // 返回true代表任务执行成功
        return true;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::merge_task";
        throw;
    }
}

bool Widget_All_Attri_Show::time_task(const QStringList &dir_path)
{
    try {
        // 对原始数据进行处理和分析timc -> target_file，获取目标文件的绝对路径
        // 【传入输入文件的路径，传出输出文件的路径】
        const string target_file_path = src_file_manager->time_task(dir_path);

        // 打开目标文件后，输入的文件名才会被修改
        // 设置Widget的窗口名称
        this->setWindowTitle(get_window_title().c_str());

        // 输入文件路径获取成功
        ifstream ifs = src_file_manager->input_file_open(target_file_path);
        // 解析生成的target_file数据读入程序中
        // timc文件的文件内容比较特殊，需要特殊的time_task函数进行处理
        datas->time_task(ifs);
        // 初始化scatter_time_sites,time_site_max_parts
//        profile_scatter_time_sites();
//        profile_time_site_max_parts();

        // 分析ration文件
        const string ration_target_file_path = src_file_manager->ration_task();
        cout << ration_target_file_path << endl;
        ifs = src_file_manager->input_file_open(ration_target_file_path);
        ration_datas->time_task(ifs);


        // 开始对数据循环扫描进行绘画
        time_while_draw();

        // 关闭打开的文件
        ifs.close();

        // 返回true代表任务执行成功
        return true;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::time_task";
        throw;
    }
}

//vector<int> Widget_All_Attri_Show::get_scatter_sites() noexcept
//{
//    return scatter_sites;
//}

//vector<string> Widget_All_Attri_Show::get_scatter_time_sites() noexcept
//{
//    return this->scatter_time_sites;
//}

//int Widget_All_Attri_Show::get_time_site_max_parts() noexcept
//{
//    return this->time_site_max_parts;
//}
