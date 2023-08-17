#include "widgets/widget_temperature_show/include/Widget_Temperature_Show.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QSpacerItem>

extern size_t pic_pages;
extern int NULL_Number;

QString test_plan_path = "";

void init_test_plan()
{
    /*
        只执行一次的函数
    */
    static bool is_first_testplan = true;
    if(is_first_testplan)
    {
        // 初始化[默认]的test_plan的路径
        test_plan_path =
                QCoreApplication::applicationDirPath() + "/" +
                "test_plan.csv";
        is_first_testplan = false;
    }
}

Widget_Temperature_Show::Widget_Temperature_Show(QWidget *parent)
    : QWidget(parent)
{
    try {
        init_test_plan();

        // 构造函数中只做一次性构造对象的
        resize(800,800);
        src_file_manager = new File_To_Targetfile;
        datas = new Targetfile_Temperature_Valid_Data;
        // 初始化step分配器
        y_stepor = new Axis_Y_Step();

        // 提娜佳布局管理器
        this->pGridLayout = new QGridLayout(this); // 添加布局到主窗体中
        this->pGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 设置网格布局管理器的一格的最小和最大大小
        this->pGridLayout->setHorizontalSpacing(0);
        this->pGridLayout->setVerticalSpacing(0);


    } catch (...) {
        qDebug() << "Widget_Temperature_Show::Widget_Temperature_Show";
        throw;
    }
}

Widget_Temperature_Show::~Widget_Temperature_Show()
{


}

bool Widget_Temperature_Show::total_task(const QStringList &files_path)
{
    try {
        // 分析源文件，获取temperature的target文件
        const string target_file_path = src_file_manager->temperature_task(files_path);
        this->setWindowTitle(get_window_title().c_str());

        // 解析生成的target_file数据读入程序中
        datas->total_task(target_file_path);

        // 开始对数据循环扫描进行绘画
        while_draw();

        // 返回true代表任务执行成功
        return true;

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::total_task";
        throw;
    }
}

const string Widget_Temperature_Show::get_window_title() noexcept
{
    // 由于不同文件夹中会有同名文件，因此使用绝对路径的方式加以文件区分
    return "Analysis file：" + src_file_manager->get_input_file_path();
}

void Widget_Temperature_Show::while_draw(int row_obj_nums)
{
    try {
        // 获取attri -> [site][part]的series数据,存于map<string,vector<vector<double>>>中
        map<string,map<string,map<string,double>>> series_datas = datas->get_series_datas();

        // 获取target_file中有效的属性label
        auto labels = datas->get_labels();

        // 产生存储照片文件
        // 将文件夹下的内容清空
        QString pic_dir = build_pic_dir();
        // 如果不是第一页开始，则不清空文件内容
        if(pic_pages == 1) clear_pic_dir(pic_dir);


        // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
        // 将chart表格依次添加入主框体中，依次显示
        size_t i;
        for(i = 0;i < labels.size();i++) // labels.size()
        {
    //        auto x = series_datas.find(labels[i]); // x->first报错，不知道为什么
            string attri = labels[i];
            // 获取site_part的对应数据点
            QMap<string,QMap<string,QPointF>> site_points = get_matrix_pointF(series_datas[attri],
                                                                           datas->get_scatter_site(),
                                                                           datas->get_scatter_part()
                                                                            );

            // 获取初始化好的chart
            Chart_Category_Value* chart = this->initChart(attri,site_points,
                                           datas->get_scatter_site(),
                                           datas->get_scatter_part()
                                           );
            // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
            this->pGridLayout->addWidget(chart,i/row_obj_nums+1,i%row_obj_nums+1);

            // 存储并保存
            // 填充pic 【会进行覆盖】
            Chart_Category_Value* pic_chart = this->initChart(attri,site_points,
                                             datas->get_scatter_site(),
                                             datas->get_scatter_part(),
                                             2);
            bulid_pic_save(pic_chart,i,pic_dir);
        }
//        qDebug() << "i: " << i;
        // 最后一页是否被存储
        bulid_pic_save(nullptr,i,pic_dir,true);


    } catch (...) {
        qDebug() << "Widget_Temperature_Show::while_draw";
        throw;
    }
}

Chart_Category_Value *Widget_Temperature_Show::initChart(const string &attri,
                                          const QMap<string, QMap<string, QPointF>> &series_datas,
                                          const vector<string> &scatter_site,
                                          const vector<string> &scatter_part,
                                          int pic_choice,
                                          const QString& _xname,
                                          const string& x_axis_unit)
{
    try {
        //设置表头【attri的名称已经在这里设置给chart作为表名了】
        Chart_Category_Value* chart =
                new Chart_Category_Value(this,attri.c_str(),pic_choice); // 局部对象

        //设置坐标系
        // 设置Y轴数据
        // 获取属性单位
        QString unit = QString::fromStdString(datas->get_attri_unit(attri));
        // 获取属性最值
        pair<double,double> attri_define_XI = datas->get_attri_define_XI(attri);
        // 获取数值最值
        pair<double,double> attri_value_XI = datas->get_attri_value_XI(attri);

        // 【构造坐标轴】
        map<string,double> y_steps = y_stepor->get_steps();
        double step = y_steps[attri]; // 获取项目的step, step一定是正数
        // 如果step没有设置，则默认step为1，如果需要修改其step，则会在step文件中意识到没有这个step
        if(step == 0)
        {
           step = 1;
        }

        // 获取define，value最值线比较的代理
        auto XI_proxy_data = profile_define_value_XI_proxy(attri_define_XI,attri_value_XI);

        // 计算y轴需要的一切接口数据
        int lines = 12;
        pair<double,double> y_axis_around = {0,0};
        profile_y_axis_ans(attri_value_XI,XI_proxy_data,step,
                           y_axis_around,lines // 传出参数
                           );

        // 获取part映射对象
        map<string,int> part_map = datas->get_part_map();

        // 设置坐标系的数值范围
        chart->setAxis(
                    // 横坐标
                    _xname,part_map,scatter_part,scatter_part.size(),
                    // 纵坐标
                    unit,y_axis_around.first,y_axis_around.second,
                    // 纵坐标的分割线的条数
                    lines,
                    pic_choice,x_axis_unit);

        //绘制【注入数据点数值和最值】
        // 这里传入XI_line_data，为了判断是否需要画图；传入attri_XI，才是真正的最值线的数据【最值有数值就画，没有就不画】
        chart->buildChart(scatter_site,scatter_part,part_map,series_datas,
                          XI_proxy_data,attri_define_XI,pic_choice);

        return chart;

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::initChart";
        throw;
    }
}


QMap<string,QMap<string,QPointF>> Widget_Temperature_Show::get_matrix_pointF(
        map<string, map<string, double>> &site_part_vals,
        const vector<string> &scatter_site,
        const vector<string> &scatter_part)
{
    try {
        // 返回值更换类型
        // 获取sites
        QMap<string,QMap<string,QPointF>> ans;
        // 初始化ans
        for(string site : scatter_site)
        {
            for(string part : scatter_part)
            {
                // 初始化所有数据为(0,0)作为数据为NULL时的断点标志
//                ans.insert(site,QVector<QPointF>(site_max_parts+1,QPointF(0,0)));
                ans[site][part] = QPointF(0,0);
            }
        }

        // 将原本site_part_vals的数据:
        //      attri -> [site][part] = val ----> attri -> [site][part] = (part,val)
        // 只是将内部值转换为QPointF(列,数值) 并将 vector转换为QVector
        // 获取映射表
        map<string,int> part_map = datas->get_part_map();
        // 遍历site_part_vals
        for(size_t i = 0;i < scatter_site.size();i++)
        {
            string site = scatter_site[i];

            // 遍历所有parts
            for(size_t j = 0;j < scatter_part.size();j++)
            {
                string part = scatter_part[j];
                // 如果为NULL值,则用(0,0)作为断点标志
                if(site_part_vals[site][part] == NULL_Number) continue;
                // 如果正常数值，则存储将 val -> (part,val)
                // 为了可以均分，这里将温度数值进行映射
                ans[site][part] = QPointF(part_map[part],site_part_vals[site][part]);
            }
        }

//        test_for(ans);

        return ans;

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::get_matrix_pointF";
        throw;
    }
}

pair<double, double> Widget_Temperature_Show::profile_define_value_XI_proxy(pair<double,double> attri_define_XI,
                                                                            pair<double,double> attri_value_XI
                                                                            )
{
    /*
        说明：
            获取define和value的XI比较后的结果，作为两者最值比较的一个代理
    */
    try {
        // 经过attri_XI和all_attri_XI对比后，综合获取的数值的最值线的数值结果
        auto XI_proxy_data = attri_define_XI;

        // 获取define和value的XI比较后的结果
        if(attri_define_XI.first != INT_MIN && attri_value_XI.first == INT_MIN)
        {
            XI_proxy_data.first = attri_value_XI.first;
        }
        // 如果数据有数据，但是最值没数据，那就不画最值线
        if(attri_define_XI.first == INT_MIN && attri_value_XI.first != INT_MIN)
        {
            // 对线进行倍数移动
            XI_proxy_data.first = attri_value_XI.first;
        }
        // 最大值构造
        if(attri_define_XI.second != INT_MAX && attri_value_XI.second == INT_MAX)
        {
            XI_proxy_data.second = attri_value_XI.second;
        }
        if(attri_define_XI.second == INT_MAX && attri_value_XI.second != INT_MAX)
        {
            // 对线进行倍数移动
            XI_proxy_data.second = attri_value_XI.second;
        }

        return XI_proxy_data;

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::profile_define_value_XI_proxy";
        throw;
    }
}

void Widget_Temperature_Show::profile_y_axis_ans(const pair<double, double> &attri_value_XI,
                                                 const pair<double, double> &XI_proxy_data,
                                                 double step,
                                                 pair<double, double> &y_axis_around,
                                                 int &lines)
{
    try {
        // 特殊情况，没有数值
        if(XI_proxy_data.first == INT_MIN && XI_proxy_data.second == INT_MAX)
        {
            y_axis_around = make_pair(-1.1,1.1);
            lines = 12;
        }
        else {
            // 获取cover数值
            auto cover_data = make_pair(
                        min(attri_value_XI.first,XI_proxy_data.first),
                        max(attri_value_XI.second,XI_proxy_data.second)
                        ) ;

            int add_n = 2;
//            qDebug() << step;
            lines = profile_y_axis_line_nums(cover_data,step); // 算cover住的实际线数
            // 更新一次可能因为四舍五入而多增加的线造成的数值变更
            cover_data.second = cover_data.first + (lines - 1) * step;

            // 留出add_n空余
            cover_data.first -= abs(add_n*step);
            cover_data.second += abs(add_n*step);

            // 更新线的个数
            lines += add_n * 2;

            y_axis_around = cover_data;

        }

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::profile_y_axis_ans";
        throw;
    }
}

int Widget_Temperature_Show::profile_y_axis_line_nums(const pair<double, double> &y_axis_XI,
                                                      double step)
{
    /*
        说明：
            step一定是正数；
            返回的线数一定可以cover住y_axis_XI中的最值数据
    */
    try {
        int factor = 100000; // 由于精度损失，造成浮点数取整出错，通过转换为整数后进行运算
        unsigned long long interval = abs(y_axis_XI.second - y_axis_XI.first) * factor;
        unsigned long long t_step = step * factor;
        int line_nums = 0;

        // 向上取整线的条数
        int dex = interval / t_step;
        // 可以取整
        if(dex * t_step == interval)
        {
            line_nums = dex;
        }
        // 向上取整
        else {
            line_nums = dex + 1; // step非1时的特殊情况
        }

        // 都需要++，就可以实现数据线对数据的完全cover
        line_nums++;

        return line_nums;

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::profile_y_axis_line_nums";
        throw;
    }
}

const QString Widget_Temperature_Show::build_pic_dir()
{
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
        qDebug() << "Widget_Temperature_Show::build_pic_dir";
        throw;
    }
}

void Widget_Temperature_Show::clear_pic_dir(const QString &pic_dir)
{
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
        qDebug() << "Widget_Temperature_Show::clear_pic_dir";
        throw;
    }
}

void Widget_Temperature_Show::bulid_pic_save(Chart_Category_Value* pic_chart,
                                              int chart_no, QString pic_dir,
                                            bool last_pic_flage,
                                              QString pic_prefix_name,
                                              int page_row_obj_nums,
                                              int page_row_nums
                                              )
{
    try {
        // 【初始化图片存储参数】
        // 初始化pic窗体和其存储布局
        static bool is_first_flag = true;
        static QWidget* pic = nullptr;
        static QGridLayout* pic_layout = nullptr;

        if(is_first_flag)
        {
            pic = new QWidget;
            pic_layout = new QGridLayout(pic);
            is_first_flag = false;
        }

        pic_layout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 设置网格布局管理器的一格的最小和最大大小
        pic_layout->setHorizontalSpacing(0);
        pic_layout->setVerticalSpacing(0);

        // 计算一张page的chart个数
        int page_charts = page_row_obj_nums * page_row_nums;

        // normal pic保存
        if(!last_pic_flage)
        {
            // 计算chart在pic中的位置
            int row = ((chart_no/page_row_obj_nums) % page_row_nums + 1);
            int col = (chart_no % page_row_obj_nums + 1);
            pic_layout->addWidget(pic_chart,row,col);

            // 6个chart一存储
            if((chart_no + 1) % page_charts == 0)
            {
                // 获取pic的存储路径
                QString path = pic_dir + "/" + pic_prefix_name +
                        // 计算当前是第几页
                        QString::fromStdString(to_string(pic_pages))+
                        QString(".png");
//                qDebug() << path;
                save_pic(pic,path);

                // 页数++
                pic_pages++;

                // 回收临时堆区内存
                delete pic_layout;
                delete pic;

                pic = new QWidget;
                pic_layout = new QGridLayout(pic);
            }
        }
        // 最后一张图片的保存
        else {
            if(chart_no % page_charts != 0) // i出来时已经i++了，这里不需要i+1
            {
                // 没有存储，则将最有一页存储
                // 获取pic的存储路径
                QString path = pic_dir + "/" + pic_prefix_name +
                        // 计算当前是第几页
                        QString::fromStdString(to_string(pic_pages))+
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
            pic_layout = nullptr;
            delete pic;
            pic = nullptr;
            is_first_flag = true;
        }


    } catch (...) {
        qDebug() << "Widget_Temperature_Show::save_normal_pic";
        throw;
    }
}

void Widget_Temperature_Show::save_pic(QWidget *saved_widget, const QString &saved_path)
{
    try {
        // 抓取窗口生成图片并保存
        QPixmap pixMap_ = QPixmap::grabWidget(saved_widget);
        pixMap_.save(saved_path);

    } catch (...) {
        qDebug() << "Widget_Temperature_Show::save_pic";
        throw;
    }
}

