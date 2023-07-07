#include "mainwindow.h"
#include "ui_mainwindow.h"

// 获取被分析的excel文件名
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
    ui->setupUi(this);
    this->setWindowTitle(tackel_file_name().c_str()); // 设置MainWindow的窗口名称
    //为MainWindow添加一个中心窗口布局【一个mianwindow只能有一个】
//    this->pHLayout = new QHBoxLayout(ui->centralwidget);
    this->pGridLayout = new QGridLayout(ui->centralwidget); // 添加布局到主窗体中
    this->draw();
//    QPixmap pixMap_ = QPixmap::grabWidget(this);
//    pixMap_.save("./myImage.png");
//    initChart();
}

// NULL的取固定值
const int NULL_Number = INT_MIN;

// 所有属性的最大、最小值和属性
// 只定义结构，而不定义对象
class UL_Unit_NUMBER
{
public:
    // 无参构造
    explicit UL_Unit_NUMBER()
    {
        // 初始化m_uls
        m_uls.insert(make_pair(LimitL,vector<double>()));
        m_uls.insert(make_pair(LimitU,vector<double>()));
    }

// string代表Limit的UL，vector中依次对应所测属性的数值【通过[下标]索引实现】
// 1. 这里将ul和unit耦合在一起，但这并不是一种耦合，因为ul与unit之间有直接的关系，大部分应用场景需要他们同时出现【有关系】
// 2. units作为单位的容器，不需要使用map的方式来占用更多的空间来实现【从效率的角度，不需要unit与ul使用相同的容器进行存储】【有效率】
map<string,vector<double>> m_uls;
vector<string> m_units; // Unit

private:
    string LimitL = "LimitL";
    string LimitU = "LimitU";

};

// 芯片号和轮数
class SITE_PART
{
public:
    // 无参构造
    explicit SITE_PART()
    {
        // 初始化m_site_part
        m_site_part.insert(make_pair(SITE_NUM,vector<int>()));
        m_site_part.insert(make_pair(PART_ID,vector<int>()));
    }

    int get_Max_Site_Number()
    {
        vector<int> v = this->m_site_part[SITE_NUM];
        int ans_Number = -1;
        for(auto x : v)
        {
            if(ans_Number < x)ans_Number = x;
        }
        return ans_Number;
    }

    int get_Max_Part_Id()
    {
        vector<int> v = this->m_site_part[PART_ID];
        int ans_Number = -1;
        for(auto x : v)
        {
            if(ans_Number < x)ans_Number = x;
        }
        return ans_Number;
    }

// string代表SITE_NUM和PART_ID，vector中依次对应所测属性的数值【通过[下标]索引实现】
map<string,vector<int>> m_site_part; // 直接写m_在类中声明的好处是，对象可以通过m_的代码提示直接查看到public中，对外开放的所有可用属性

private:
    string SITE_NUM = "SITE_NUM";
    string PART_ID = "PART_ID";

};

MainWindow::~MainWindow()
{
    delete ui;
}


// 获取PART_ID最大值【范围】
int get_PART_ID()
{

}

// 获取属性的最大最小值
void get_attri_MAX_MIN(double& m_max,double& m_min)
{

}


// 判断一个string是否为整数数字
inline bool is_Numeric(const string &str)
{
    auto it = str.begin();
    while (it != str.end() && std::isdigit(*it)) {
        it++;
    }
    return !str.empty() && it == str.end();
}

// 获取数据的vector<vector<double>>>的map数据【将string转换为double存储在矩阵中】
void draw_map
(const vector<vector<string>>& target_vec, // 数据源
    SITE_PART& site_parts,
    UL_Unit_NUMBER& uuls,
     map<string,vector<vector<double>>>& ans) // 传入参数
{
    // 获取site和part -> site_parts
    for(size_t i = 0;i < 2;i++)
    {
        string attri;
        for(size_t j = 0;j < target_vec[i].size();j++)
        {
            if(j == 0) attri = target_vec[i][j];
            if(is_Numeric(target_vec[i][j]))
            {
                site_parts.m_site_part[attri].push_back(stoi(target_vec[i][j]));
            }
        }
    }

    // 获取unit,limitL和limitU -> uuls
    for(size_t i = 0,j = 0;j < target_vec[i].size();j++)
    {
        string attri;
        if(target_vec[i][j] != "Unit" || target_vec[i][j] != "LimitL" || target_vec[i][j] != "LimitU") continue;

        // 获取属性名称
        attri = target_vec[i][j];
        for(size_t k = 2;k < target_vec.size();k++)
        {
            if(attri == "Unit") uuls.m_units.push_back(target_vec[k][j]);
            else {
                // 如果为空的话，获得一个计算后的数值进行存储【！！！】
                uuls.m_uls[attri].push_back(stod(target_vec[k][j]));
           }
        }
    }

    // 获取主数据群
    for(size_t i = 2;i < target_vec.size();i++)
    {
        string attri;
        vector<vector<double>> data(); // 【！！！】初始化大小后，才能矩阵随机的存储【使用类内定义好的函数返回值】
        bool flage = true; // 观念值【就像电路板上的0 1数据代表的含义一样，给数据赋予自己所谓的意义，执行对应操作】
        for(size_t j = 0; j < target_vec[i].size();j++)
        {
            // 跨越无效数据区
            if(j == 0) attri = target_vec[i][j];
            if(target_vec[i][j].size() != 0 && flage) continue;
            if(target_vec[i][j].size() == 0) {
                flage = false;
                continue;
            }
            // 正式读取主数据群数据

        }
    }

//    map<string,vector<vector<double>>> m;
//    map<string,vector<double>> mv;
//    string str;vector<vector<double>> vec;
//    for(int i = 0;i < target_vec.size();i++)
//    {
//        for(int j = 0;j < target_vec[i].size();j++)
//        {
//            if(i == 0 && j == 0) {
//                str = target_vec[0][0];
//                continue;
//            }
//            vec.push_back(stod(target_vec[i][j]));

//        }
//    }
}

// 制作QList<QList<QPointF>>数据列表
QList<QList<QPointF>> get_QList(vector<vector<double>> data)
{

}

// 获取target表的所有结果
vector<vector<string>> load_target_vec(const ifstream& ifs)
{
    return tackle_file_all(ifs);
}


//初始化图表
Chart* MainWindow::initChart(const string& label,const vector<vector<double>>& data)
{
    //设置表头
    chart = new Chart(this,label.c_str());
    //为MainWindow的centralWidget添加一个布局
//    QHBoxLayout *pHLayout = new QHBoxLayout(ui->centralwidget);

    //设置坐标系
    chart->setAxis("PART_ID",1,100,11, label.c_str(),0,20,11);

    //设置离散点数据
    QList<QPointF> pointlist = {QPointF(0,1), QPointF(10,2), QPointF(20,4), QPointF(30,8), QPointF(40,16),
                                QPointF(50,16), QPointF(60,8), QPointF(70,4), QPointF(80,2), QPointF(90,1),};

//    QList<QList<QPointF>> pointlist1;
    //绘制
    chart->buildChart(pointlist);

    //将chart添加到布局中【多个chart显示在一个窗口中，合理排布】
//    pHLayout->addWidget(chart);

    return chart;

//        // 设置表头
//        chart = new Chart(this,tackel_file_name().c_str());
//        //设置坐标系
//        chart->setAxis("X轴",0,100,11, "Y轴",0,20,11);
//        //为MainWindow的centralWidget添加一个布局
//        QHBoxLayout *pHLayout = new QHBoxLayout(ui->centralwidget);
}

void MainWindow::while_draw(const ifstream& ifs)
{
    /*循环添加所有生成的chart到网格布局管理器中*/
    // 加载ifs中的所有内容进入程序
    vector<vector<string>> target_vec = load_target_vec(ifs);
    // 获取map<string,vector<vector<double>>>的数据
    map<string,vector<vector<double>>>m =  draw_map(target_vec);

    // 将map数据按照labels中，循环获取key和value后，传入initChart创建对应属性的chart
        // 将chart表格依次添加入主框体中，依次显示
    for(size_t i = 0;i < labels.size();i++)
    {
        auto x = m.find(labels[i]);
        Chart* chart = this->initChart(x->first,x->second);
        this->pGridLayout->addWidget(chart,i/5+1,i%5+1);
    }
    //    for(int i =1;i<=3;i++)
    //    {
    //        for(int j = 1;j <= 5; j++)
    //        {
    //            this->pGridLayout->addWidget(initChart(),i,j);
    //        }
    //    }

}

void MainWindow::draw()
{
    /*使用中间文件，将目标文件的数据打开后，对其中内容进行绘制*/
    // 数据处理
    const string draw_File_Name = task01();
    ifstream ifs = input_file_open(draw_File_Name);


    // 开始绘画
//    cout<<"drawing..."<<endl;
    while_draw(ifs);
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

