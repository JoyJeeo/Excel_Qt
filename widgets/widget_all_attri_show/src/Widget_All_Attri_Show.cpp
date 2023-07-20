#include "../include/Widget_All_Attri_Show.h"


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

void Widget_All_Attri_Show::profile_scatter_sites()
{
    /*
        获取scatter_sites，其是该类的一个属性，由private函数进行维护
    */
    try {
        scatter_sites = datas->get_site_parts().get_Scatter_Site_Number();
//        qDebug() << scatter_sites.size();
//        exit(1);
    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_scatter_sites";
        throw;
    }
}

void Widget_All_Attri_Show::profile_site_max_parts()
{
    try {
       site_max_parts = datas->get_site_parts().get_Max_Part_Id();
    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_site_max_parts";
        throw ;
    }
}

void Widget_All_Attri_Show::profile_site_list()
{
    try {
        site_list = datas->get_site_parts().get_site_list();
    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::profile_site_list";
        throw;
    }
}

void Widget_All_Attri_Show::test_for(QMap<int,QVector<QPointF>> &datas)
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
        this->pGridLayout = new QGridLayout(this); // 添加布局到主窗体中
        this->pGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize); // 设置网格布局管理器的一格的最小和最大大小

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::Widget_All_Attri_Show";
        throw;
    }
}

Widget_All_Attri_Show::~Widget_All_Attri_Show()
{

}

// 制作QList<QList<QPointF>>数据列表
QMap<int,QVector<QPointF>> Widget_All_Attri_Show::get_matrix_pointF(map<int,vector<double>>& site_part_vals)
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
                             double axisX_k,
                             double axisY_k)
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

        // 获取的数值的最值线的数值结果
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
                    "PART_ID",1,site_max_parts,site_max_parts,
                    // 纵坐标
                    unit,realY_XI.first,realY_XI.second,
                    // 纵坐标的分割线的条数
                    12
                    );
        qDebug() << "3";
        qDebug() << "scatter_sites.size(): " << scatter_sites.size();
        //绘制【注入数据点数值和最值】
        chart->buildChart(scatter_sites,site_max_parts,site_points,XI_line_data);

        return chart;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::initChart";
        throw;
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
        if(attri_XI.first != INT_MIN && all_attri_XI.first == INT_MIN)
        {
            XI_line_data.first = all_attri_XI.first;
        }
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

        // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
        // 将chart表格依次添加入主框体中，依次显示
        for(size_t i = 0;i < labels.size();i++) // labels.size()
        {
    //        auto x = series_datas.find(labels[i]); // x->first报错，不知道为什么
            string attri = labels[i];
            // 获取site_part的对应数据点
            QMap<int,QVector<QPointF>> site_points = get_matrix_pointF(series_datas[attri]);
            qDebug() << "2";
            // 获取初始化好的chart
            Chart* chart = this->initChart(attri,site_points);
            // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
            this->pGridLayout->addWidget(chart,i/row_obj_nums+1,i%row_obj_nums+1);
        }

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
        profile_scatter_sites();
        profile_site_list();
        profile_site_max_parts();
        qDebug() << "1";

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

vector<int> Widget_All_Attri_Show::get_scatter_sites() noexcept
{
    return scatter_sites;
}
