#include "mainwindow.h"
#include "ui_mainwindow.h"

const string tackel_file_name()
{
    size_t i;

    for(i = IN_FILE_NAME.size()-1;i >=0 ;i--)
    {
        if(IN_FILE_NAME[i] == '\\')break;
    }
    string ans = "分析文件：";
    ans +=  IN_FILE_NAME.substr(i+1,IN_FILE_NAME.size()-i+1);

    return ans;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //    resize(800,800);
        ui->setupUi(this);
        this->setWindowTitle(tackel_file_name().c_str()); // 设置MainWindow的窗口名称
        //为MainWindow添加一个中心窗口布局【一个mianwindow只能有一个】
        this->pGridLayout = new QGridLayout(ui->centralwidget); // 添加布局到主窗体中
        this->draw();
    //    QPixmap pixMap_ = QPixmap::grabWidget(this);
    //    pixMap_.save("./myImage.png");
    //    initChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 制作QList<QList<QPointF>>数据列表
QVector<QVector<QPointF>> MainWindow::get_QVector(const vector<vector<double>>& site_part_vals)
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
}



//初始化图表
Chart* MainWindow::initChart(const string& attri,
                             const vector<vector<double>>& site_part_vals,
                             Valid_Data& datas,
                             double axisX_k = 1,
                             double axisY_k = 0.01)
{
    /*
        这里设置y轴的放大倍数默认为1.2,留一个接口,为数据图像的放大和缩小做准备
            [图像的方法/缩小,本质就是数轴的放大和缩小]
    */
    //设置表头
    chart = new Chart(this,attri.c_str());

    // 获取site_part
    auto site_part = datas.get_site_parts();
    // 获取attri_uul
    auto attri_uul = datas.get_attri_uuls();

    //设置坐标系
    // 获取PART_ID的最大值
    size_t PART_ID_MAX = site_part.get_Max_Part_Id();
    // 获取属性的单位
    string unit = attri_uul.m_attri_uuls[attri].m_Unit;
    // 获取属性的最值:是LimitL,LimitU与属性值比较后获得的最值[为了使得图表画出的线不会超出屏幕范围]
//    auto ul_cmp_attri_XI = datas.get_ul_compare_attri_XI(attri);
    // 对ul_cmp_attri_XI的最值进行进一步处理,由于其数据范围为[INT_MIN,INT_MAX]
    //      1. 如果是正常数值,则需要在原本最值的基础上扩大一部分数值[方便观察],这里默认扩大1/5
    //      2. 如果是INT_*数据,则使用默认[-1.10,1.10]进行设置
//    auto realY_XI = make_pair(
//                ul_cmp_attri_XI.first == INT_MIN ? -1.10 : ul_cmp_attri_XI.first * axisY_k,
//                ul_cmp_attri_XI.second == INT_MAX ? 1.10 : ul_cmp_attri_XI.second * axisY_k
//                );
    auto attri_XI = datas.get_attri_XI(attri);
    // 设置坐标系的数值范围
    chart->setAxis(
                // 横坐标
                "PART_ID",1,PART_ID_MAX,PART_ID_MAX,
                // 纵坐标
                unit.c_str(),attri_XI.first * (1 - axisY_k),attri_XI.second * (1 + axisY_k),
                // 纵坐标的分割线的条数
                    11
                );
//            unit.c_str(),0.504,0.512,
//            // 纵坐标的分割线的条数
//            10
//            );

    // 获取site_part的对应数据点
    QVector<QVector<QPointF>> point_vecs = get_QVector(site_part_vals);

    //绘制
    chart->buildChart(point_vecs);

    return chart;

//        // 设置表头
//        chart = new Chart(this,tackel_file_name().c_str());
//        //设置坐标系
//        chart->setAxis("X轴",0,100,11, "Y轴",0,20,11);
//        //为MainWindow的centralWidget添加一个布局
//        QHBoxLayout *pHLayout = new QHBoxLayout(ui->centralwidget);
}

void MainWindow::while_draw(Valid_Data datas)
{
    /*循环添加所有生成的chart到网格布局管理器中*/

    // 获取attri -> [site][part]的series数据,存于map<string,vector<vector<double>>>中
    map<string,vector<vector<double>>>series_datas = datas.get_series_datas();

    // 获取target_file中有效的属性label
    auto labels = datas.get_labels();

    // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
    // 将chart表格依次添加入主框体中，依次显示
    for(size_t i = 0;i < 10;i++) // labels.size()
    {
//        auto x = series_datas.find(labels[i]); // x->first报错，不知道为什么
        // 从label中的第二个开始算
        int no = i+2;
        Chart* chart = this->initChart(labels[no],series_datas[labels[no]],datas);
        // (widget,row,col) 物件和在网格布局管理器中的横纵坐标位置
        this->pGridLayout->addWidget(chart,i/5+1,i%5+1);
    }

}

void MainWindow::draw()
{
    /*使用中间文件，将目标文件的数据打开后，对其中内容进行绘制*/
    // 数据处理
    // 对原始数据进行处理和分析source_file -> target_file
    const string draw_File_Name = task01();
    ifstream ifs = input_file_open(draw_File_Name);

    // 解析生成的target_file数据读入程序中
    Valid_Data datas;
    datas.load_target_vec(ifs);
    datas.profile_get_datas();

    // 开始绘画
//    cout<<"drawing..."<<endl;
    while_draw(datas);
//        for(int i =1;i<=3;i++)
//        {
//            for(int j = 1;j <= 5; j++)
//            {
//                this->pGridLayout->addWidget(initChart(),i,j);
//            }
//        }


//    this->pVLayout->addWidget(pHLayout01,1,1,1,1);

    // 关闭打开的文件
    ifs.close();
}

