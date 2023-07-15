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
        string ans = "分析文件：" + src_file_name;
        return ans;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::tackel_file_name";
        throw;
    }
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
        this->pGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        this->setWindowTitle(tackel_file_name().c_str()); // 设置Widget的窗口名称

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::Widget_All_Attri_Show";
        throw;
    }
}

Widget_All_Attri_Show::~Widget_All_Attri_Show()
{

}

// 制作QList<QList<QPointF>>数据列表
QVector<QVector<QPointF>> Widget_All_Attri_Show::get_QVector(const vector<vector<double>>& site_part_vals)
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
        QVector<QVector<QPointF>> ans;

        // 将原本site_part_vals的数据:
        //      attri -> [site][part] = val ----> attri -> [site][part] = (part,val)
        // 只是将内部值转换为QPointF(列,数值) 并将 vector转换为QVector
        for(size_t i = 0;i < site_part_vals.size();i++)
        {
            ans.push_back(QVector<QPointF>());
            for(size_t j = 0;j < site_part_vals[i].size();j++)
            {
                // 如果为NULL值,则用(0,0)作为断点标志
                if(site_part_vals[i][j] == NULL_Number)
                    ans[i].push_back(QPointF(0,0));
                // 如果正常数值，则存储将 val -> (part,val)
                else
                {
                    size_t parts = j + 1; // 将第几个part再转回来
                    ans[i].push_back(QPointF(parts,site_part_vals[i][j]));
                }
            }
        }

        return ans;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::get_QVector";
        throw;
    }
}

void Widget_All_Attri_Show::build_axis(Chart *chart)
{
    // 获取site_part
    auto site_part = datas->get_site_parts();
    // 获取attri_uul
    auto attri_uul = datas->get_attri_uuls();

    //设置坐标系
    // 获取PART_ID的最大值
    size_t PART_ID_MAX = site_part.get_Max_Part_Id();
    // 获取属性的单位
    string unit = attri_uul.m_attri_uuls[attri].m_Unit;

    // 获取最值
    // 获取属性最值
    auto attri_XI = make_pair(attri_uul.m_attri_uuls[attri].m_LimitL,
                              attri_uul.m_attri_uuls[attri].m_LimitU);
    // 获取所有属性值的最值
    auto all_attri_XI = datas->get_attri_XI(attri);

    // 获取属性最值和所有属性值对比的最值
    auto ul_cmp_attri_XI = datas->get_ul_compare_attri_XI(attri);

    // 对纵坐标的大小范围进行处理
//     对ul_cmp_attri_XI的最值进行进一步处理,由于其数据范围为[INT_MIN,INT_MAX]
//          1. 如果是正常数值,则需要在原本最值的基础上扩大一部分数值[方便观察],这里默认扩大axisY_k
//          2. 如果是INT_*数据,则使用默认[-1.10,1.10]进行设置
    auto realY_XI = make_pair(
                ul_cmp_attri_XI.first == INT_MIN ? -1.10 : ul_cmp_attri_XI.first * (1 - axisY_k),
                ul_cmp_attri_XI.second == INT_MAX ? 1.10 : ul_cmp_attri_XI.second * (1 + axisY_k)
                );
    // 设置坐标系的数值范围
    chart->setAxis(
                // 横坐标
                "PART_ID",1,PART_ID_MAX,PART_ID_MAX,
                // 纵坐标
//                unit.c_str(),attri_XI.first * (1 - axisY_k),attri_XI.second * (1 + axisY_k),
                unit.c_str(),realY_XI.first,realY_XI.second,
                // 纵坐标的分割线的条数
                    12
                );
}

//初始化图表
Chart* Widget_All_Attri_Show::initChart(const string& attri,
                             const vector<vector<double>>& site_part_vals,
                             double axisX_k = 1,
                             double axisY_k = 0.01)
{
    /*
        这里设置y轴的放大倍数默认为1.2,留一个接口,为放大镜功能做准备
            [图像的方法/缩小,本质就是数轴的放大和缩小]
    */
    try {
        //设置表头【attri的名称已经在这里设置给chart作为表名了】
        Chart* chart = new Chart(this,attri.c_str()); // 局部对象



        // 获取site_part的对应数据点
        QVector<QVector<QPointF>> point_vecs = get_QVector(site_part_vals);

        //绘制【注入数据点数值和最值】
        chart->buildChart(point_vecs,attri_XI);

        return chart;

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::initChart";
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
        map<string,vector<vector<double>>>series_datas = datas->get_series_datas();

        // 获取target_file中有效的属性label
        auto labels = datas->get_labels();

        // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
        // 将chart表格依次添加入主框体中，依次显示
        for(size_t i = 0;i < labels.size()-2;i++) // labels.size()
        {
    //        auto x = series_datas.find(labels[i]); // x->first报错，不知道为什么
            // 从label中的第二个开始算
            int no = i+2;
            Chart* chart = this->initChart(labels[no],series_datas[labels[no]]);
            // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
            this->pGridLayout->addWidget(chart,i/row_obj_nums+1,i%row_obj_nums+1);
        }

    } catch (...) {
        qDebug() << "Widget_All_Attri_Show::while_draw";
        throw;
    }

}

bool Widget_All_Attri_Show::total_task()
{
    /*使用中间文件，将目标文件的数据打开后，对其中内容进行绘制*/
    // 数据处理
    try{
        // 对原始数据进行处理和分析source_file -> target_file，获取目标文件的绝对路径
        const string draw_File_Name = src_file_manager->total_task();
        // 如果没有选择文件，则为空，直接结束
        if(draw_File_Name == "")
        {
            // 返回false代表任务执行失败
            return false;
        }

        // 输入文件路径获取成功
        ifstream ifs = src_file_manager->input_file_open(draw_File_Name);
        // 解析生成的target_file数据读入程序中
        datas->total_task(ifs);

        // 开始对数据循环扫描进行绘画
        while_draw();

        // 抓取窗口生成图片并保存
    //    QPixmap pixMap_ = QPixmap::grabWidget(this);
    //    pixMap_.save("./myImage.png");

        // 关闭打开的文件
        ifs.close();

        // 返回true代表任务执行成功
        return true;

    } catch(...){
        qDebug() <<"Widget_All_Attri_Show::total_task()";
        throw;
    }
}
