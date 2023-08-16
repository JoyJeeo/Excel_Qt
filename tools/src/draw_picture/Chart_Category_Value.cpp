#include "tools/include/draw_picture/Chart_Category_Value.h"

#include <QFont>
#include <QLegendMarker>
#include <QList>
#include <string>
#include <map>
using namespace std;

static const int pic_chart_len_size = 1500;
static const int pic_chart_hight_size = 1800;
static const int pic_chart_name_size = 50;
static const int pic_legend_size = 35;
static const int pic_axis_size = 35;

Chart_Category_Value::Chart_Category_Value(QWidget *parent, QString _chartname, int pic_choice)
    :QWidget(parent),chartname(_chartname)
{
    try {
        qchart = new QChart;
        chartview = new QChartView(qchart); // 将chart图表添加入chart显示器中

        layout = new QHBoxLayout(this); // 添加对象到布局中
        axisX = new QCategoryAxis(this);
        axisY = new QValueAxis(this);
        //在ui里面添加了一个Widget并把曲线图添加进去
        layout->addWidget(chartview); // chart显示器添加入布局中
        chartview->setRenderHint(QPainter::Antialiasing);//防止图形走样：抗锯齿 // 设置渲染效果

        // 设置chartview观察器的大小范围
//        chartview->setMinimumSize(500,500);
        switch(pic_choice)
        {
        case 0:
            // 默认为0，电脑显示A4大小
//            chartview->setMinimumSize(350,320); // A4
            chartview->setMinimumSize(350,600); // A4
            break;
        case 1:
            // 生成图片的大小
            chartview->setMinimumSize(770,320);
            break;
        case 2:
            // 更大的图片大小
            chartview->setMinimumSize(pic_chart_len_size,pic_chart_hight_size);
            break;
        }

        // 设置chartview具有放大镜功能【！！！】
        chartview->setRubberBand(QChartView::RectangleRubberBand);

    } catch (...) {
        qDebug() << "Chart_Category_Value::Chart_Category_Value";
        throw;
    }
}

Chart_Category_Value::~Chart_Category_Value()
{
    try {

    } catch (...) {
        qDebug() << "Chart_Category_Value::~Chart_Category_Value";
        throw;
    }
}

void Chart_Category_Value::setAxis(QString _xname, map<string,int> &_x_map,
                                   const vector<string> &_x_map_list,int _xtickc,
                                   QString _yname, qreal _ymin, qreal _ymax, int _ytickc,
                                   int pic_choice,const string& _x_unit)
{
    try {
        xname = _xname; x_map = _x_map; x_map_list = _x_map_list; xtickc = _xtickc;
        yname = _yname; ymin = _ymin; ymax = _ymax; ytickc = _ytickc;
        /************************************
            %u 无符号十进制整数
            %s 字符串
            %c 一个字符
            %d 有符号十进制整数
            %e 浮点数、e-记数法
            %f 浮点数、十进制记数法
            %s 字符串
        ****************************************/
        // 设置轴名称的字体大小
        QFont axis_font = QFont("Consolas");
        axis_font.setStyleStrategy(QFont::PreferAntialias);
        switch (pic_choice) {
        case 0:
            // 专门用于电脑显示
            axis_font.setPixelSize(11);
            break;
        case 1:
            axis_font.setPixelSize(15);
            break;
        case 2:
            // 存储更大的图片
            axis_font.setPixelSize(pic_axis_size);
            break;
        }
        // 设置轴名称
        axisX->setLabelsFont(axis_font);
        axisY->setLabelsFont(axis_font);

        // 设置轴上刻度数字的字体和大小
        QFont font = QFont("Consolas");
//        QFont font;
        font.setStyleStrategy(QFont::PreferAntialias);
//        font.setPointSize(1);
//        font.setPointSize(11);
        switch (pic_choice) {
        case 0:
            // 专门用于电脑显示
            font.setPixelSize(11);
            break;
        case 1:
            font.setPixelSize(15);
            break;
        case 2:
            // 存储更大的图片
            font.setPixelSize(pic_axis_size);
            break;
        }
        font.setBold(true);


        // 设置X轴描述
        axisX->setGridLineVisible(true);   //x轴网格线可见
        axisX->setLabelFormat("%s");   //设置x轴的刻度标签格式
        axisX->setTitleText(xname);  //设置x轴的描述
        axisX->setTitleFont(font);
        axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue); // 设置标签的位置
        //设置x轴的数据范围
        build_x_axis(_x_unit);
        axisX->setTickCount(xtickc);       //x轴设置实网格线的数量【也是一个大格】
        axisX->setMinorTickCount(0);   //设置x轴每个大格里面小刻度线的数目

        // 设置Y轴描述
        axisY->setGridLineVisible(true); // y轴网格线可见
        axisY->setLabelFormat("%0.3f"); // 设置y轴的刻度标签格式
        axisY->setTitleText(yname); // 设置y轴的描述
        axisY->setTitleFont(font);
        axisY->setRange(ymin, ymax); // 设置y轴的数据范围
        axisY->setTickCount(ytickc); // y轴设置实网格线的数量【也是一个大格】
        axisY->setMinorTickCount(1); // 设置y轴每个大格里面小刻度线的数目
        // 设置刻度线
        axisY->setTickType(QValueAxis::TickType::TicksFixed);


        // 将数轴添加入chart图表中
        qchart->addAxis(axisX, Qt::AlignBottom); //下：Qt::AlignBottom  上：Qt::AlignTop
        qchart->addAxis(axisY, Qt::AlignLeft);   //左：Qt::AlignLeft    右：Qt::AlignRight

    } catch (...) {
        qDebug() << "Chart_Category_Value::setAxis";
        throw;
    }
}

void Chart_Category_Value::buildChart(const vector<string> &scatter_site,
                                      const vector<string> &scatter_part,
                                      map<string,int> &part_map,
                                      const QMap<string, QMap<string,QPointF>> &series_data,
                                      const pair<double, double> &XI_proxy_data,
                                      const pair<double, double> &attri_define_XI,
                                      int pic_choice)
{
    try {
        // 设置chart名称
        QFont font = QFont("Consolas");
        switch (pic_choice) {
        case 0:
            // 专门用于电脑显示
            font.setPixelSize(15);
            break;
        case 1:
            font.setPixelSize(20);
            break;
        case 2:
            // 存储更大的图片
            font.setPixelSize(pic_chart_name_size);
            break;
        }
        font.setBold(true);
        qchart->setTitleFont(font);
        qchart->setTitle(chartname);

        // 设置数据线粗细
        int data_series_width = 3;
        // 设置最值线粗细
        int XI_series_width = 3;

        // 【特殊最值处理】：如果没有数值存在时，图表什么都不画
        if(XI_proxy_data.first == INT_MIN && XI_proxy_data.second == INT_MAX)return;

        // 绘制数据线
        construct_datas_series(scatter_site,scatter_part,series_data,data_series_width);

        // 绘制最值线 // 【最值线的绘制，只与attri_XI有关】
        construct_XI_line(attri_define_XI,XI_series_width,scatter_part,part_map);
        // 修正图例样式
        construct_legend_style(scatter_site,attri_define_XI,pic_choice);

    } catch (...) {
        qDebug() << "Chart_Category_Value::buildChart";
        throw;
    }
}

void Chart_Category_Value::build_x_axis(const string& x_unit)
{
    try {

        for(string part : x_map_list)
        {
            axisX->append(QString::fromStdString(part + x_unit),x_map[part]);
        }

    } catch (...) {
        qDebug() << "Chart_Category_Value::build_x_axis";
        throw;
    }
}

void Chart_Category_Value::construct_datas_series(const vector<string> &scatter_site,
                                                  const vector<string> &scatter_part,
                                                  const QMap<string, QMap<string,QPointF>> &series_data,
                                                  int data_series_width)
{
    try {
        // 填充series中的数据
        // 遍历属性下的每组芯片
        qreal zero = 0.0;

        for(size_t i = 0;i < scatter_site.size();i++)
        {
            string site = scatter_site[i];
//            qDebug() << site;
            // 起步：
            // 创建第一个线组
            QVector<QLineSeries*> t_line_vec;
            // 创建第一个点组
            QLineSeries* line = new QLineSeries(this);
            // 初始化点组上生成线时的初始化数据
            line->clear();
            line->setPen(QPen(this->colors[i],data_series_width,Qt::SolidLine));
            // 将点组加入线组中
            t_line_vec.push_back(line);
            // 将点组容器加入线组容器中
            series.insert(site,t_line_vec);
            // 记录当前线组的序号
            int grp = 0;

            // 遍历每组芯片内的点数据
            for(size_t j = 0;j < scatter_part.size();)
            {
                string part = scatter_part[j];
                // 如果不为NULL点，则点不为(0,0)，正常添加点数据
                if(series_data[site][part].x() != zero || series_data[site][part].y() != zero)
                {
                    series[site][grp]->append(series_data[site][part]);
                    series[site][grp]->setPointsVisible(); // 将具体点标注出来
//                    series[site][grp]->setPointLabelsVisible(); // 点上出现具体数值
                    j++;
                }
                // 如果为(0,0)点，则创建一个新的点组进行记录【实现分段处理】
                else {
                    // 连续的(0,0)间断，需要进行跨越
                    while(series_data[site][part].x() == zero && series_data[site][part].y() == zero)
                    {
                        // 防止连续++造成越界
                        j++;
                        if(j >= scatter_part.size()) break;
                        part = scatter_part[j];
                    }
                    // 没有数据点，不需要去构造线【当part已经越界时】
                    if( j < scatter_part.size())
                    {
                        // 跳过了连续的(0,0)点
                        QLineSeries* t_line = new QLineSeries(this);
                        t_line->clear();
                        t_line->setPen(QPen(this->colors[i],data_series_width,Qt::SolidLine));
                        series[site].push_back(t_line);
                        grp++;
                    }
                }
            }
        }

        // 遍历series中的每条线，将生成好的多组线加入chart中
        // 直接遍历LineSeries 加入chart即可，因为line的颜色已经区分了每组site，
        //      因此间断的line并不会影响到chart生成的线数据
        for(size_t i = 0;i < scatter_site.size();i++)
        {
            string site = scatter_site[i];
            // 这里的含义是series中的线组的part
            for(int parts = 0;parts < series[site].size();parts++)
            {
                // 添加LineSeries 加入chart
                qchart->addSeries(series[site][parts]);
                // 设置x轴和y轴与LineSeries的点数据进行对应
                qchart->setAxisX(axisX, series[site][parts]);
                qchart->setAxisY(axisY, series[site][parts]);
            }
        }
    } catch (...) {
        qDebug() << "Chart_Category_Value::construct_datas_series";
        throw;
    }
}

void Chart_Category_Value::construct_XI_line(const pair<double, double> &attri_define_XI,
                                             int XI_series_width,
                                             const vector<string> &scatter_part,
                                             map<string,int>& part_map)
{
    try {
        // 添加最值线【都是红色的虚线】
        // 如果最大值线本身并不存在
        if(attri_define_XI.second != INT_MAX)
        {
            // 最大值线
            QLineSeries* max_line = new QLineSeries(this);
            // 初始化点组上生成线时的初始化数据
            max_line->clear();
            max_line->setPen(QPen(Qt::red,XI_series_width,Qt::DashLine));

            for(size_t j = 0;j < scatter_part.size();j++)
            {
                string part = scatter_part[j];
                max_line->append(QPointF(part_map[part],attri_define_XI.second));
            }

            qchart->addSeries(max_line);
            qchart->setAxisX(axisX, max_line);
            qchart->setAxisY(axisY, max_line);

        }

        // 如果最小值线，本身并不存在
        if(attri_define_XI.first != INT_MIN)
        {
            // 最小值线
            QLineSeries* min_line = new QLineSeries(this);
            // 初始化点组上生成线时的初始化数据
            min_line->clear();
            min_line->setPen(QPen(Qt::red,XI_series_width,Qt::DashLine));
            // 遍历每组芯片内的点数据
            for(size_t j = 0;j < scatter_part.size();j++)
            {
                string part = scatter_part[j];
                min_line->append(QPointF(part_map[part],attri_define_XI.first));
            }
            // 添加LineSeries 加入chart
            qchart->addSeries(min_line);
            // 设置x轴和y轴与LineSeries的点数据进行对应
            qchart->setAxisX(axisX, min_line);
            qchart->setAxisY(axisY, min_line);

        }

    } catch (...) {
        qDebug() << "Chart_Category_Value::construct_XI_line";
        throw;
    }
}

void Chart_Category_Value::construct_legend_style(const vector<string> scatter_site,
                                                  const pair<double, double> &attri_define_XI,
                                                  int pic_choice)
{
    try {
        // 设置线条图例
        qchart->legend()->setAlignment(Qt::AlignRight); // 设置图例靠右显示
        qchart->legend()->setBackgroundVisible(false); // 设置图例背景不可见
        qchart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries); // 设置图例的显示类型跟随曲线的类型显示
        // 设置图例描述文字的字体
        QFont font = QFont("Consolas");
//        QFont font;
        font.setBold(true);
//        font.setPointSize(10);
        switch (pic_choice) {
        case 0:
            font.setPixelSize(10);
            break;
        case 1:
            font.setPixelSize(12);
            break;
        case 2:
            font.setPixelSize(pic_legend_size);
            break;
        }
        qchart->legend()->setFont(font);

        // 获取所有图例的markers，修改图里描述内容
        QList<QLegendMarker *> legends = qchart->legend()->markers();
//        qDebug() << (QLineSeries*())

        size_t legends_size = legends.size();

        // 计算最值线的存在个数
        int max_line_num = 0;
        if(attri_define_XI.first != INT_MIN)max_line_num++;
        if(attri_define_XI.second != INT_MAX)max_line_num++;

        // 设置图例中的文字描述
        // 绘制数据图例
        // 只对数据线图例进行扫描设置
        size_t counter = 0; // 单独记录vector的问题
        for(size_t i = 0;i < legends_size - max_line_num;i++)
        {
            // site数据线的图例
            // 线的绘制顺序，与芯片的顺序是一致的，由于绘制时都使用scatter_sites作为遍历顺序的依据
            string site = scatter_site[counter++];

            // 将同一个site的多条part线跳过，只保留其中一个即可
            int scope = series[site].size();
            while(scope > 1)
            {
                legends[i]->setVisible(false);
                scope--;i++;
            }

            legends[i]->setLabel(QString::fromStdString(site));

        }

        // 绘制最值图例
        // 判断最值线的绘制图例【图例的绘制顺序与绘制最值线时有关】
        // 只有一条最值线存在时
        if(max_line_num == 1)
        {
            legends[legends_size - max_line_num]->setLabel(
                        attri_define_XI.second != INT_MAX ?
                        "max_line: " + keep_dot_next(to_string(attri_define_XI.second))  :
                        "min_line: " + keep_dot_next(to_string(attri_define_XI.first))
                        );
        }
        // 两条最值线都存在时
        if(max_line_num == 2)
        {
            legends[legends_size - max_line_num]->setLabel(
                        "max_line: " + keep_dot_next(to_string(attri_define_XI.second))
                         );
            legends[legends_size - max_line_num + 1]->setLabel(
                        "min_line: " + keep_dot_next(to_string(attri_define_XI.first))
                        );
        }

    } catch (...) {
        qDebug() << "Chart_Category_Value::construct_legend_style";
        throw;
    }
}

const QString Chart_Category_Value::keep_dot_next(const string &num, int keep_num)
{
    try {
        int dot_dex;
        for(dot_dex = num.size() - 1;dot_dex >= 0;dot_dex--)
        {
            if(num[dot_dex] == '.')break;
        }

        QString ans_str = QString::fromStdString(num.substr(0,dot_dex + keep_num + 1));

        return ans_str;

    } catch (...) {
        qDebug() << "Chart_Category_Value::keep_dot_next";
        throw;
    }
}
