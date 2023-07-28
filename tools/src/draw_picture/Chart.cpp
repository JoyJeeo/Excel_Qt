#include "tools/include/draw_picture/chart.h"

#include <QFont>
#include <QLegendMarker>
#include <QList>
#include <string>
using namespace std;

Chart::Chart(QWidget* parent, QString _chartname)
    :QWidget(parent),chartname(_chartname){
    try {
        // [先不初始化所有线]

        qchart = new QChart;
        chartview = new QChartView(qchart); // 将chart图表添加入chart显示器中
//        chartview = new My_Chart_View(qchart); // 将chart图表添加入chart显示器中
        layout = new QHBoxLayout(this); // 添加对象到布局中
        axisX = new QValueAxis(this);
        axisY = new QValueAxis(this);
        //在ui里面添加了一个Widget并把曲线图添加进去
        layout->addWidget(chartview); // chart显示器添加入布局中
        chartview->setRenderHint(QPainter::Antialiasing);//防止图形走样：抗锯齿 // 设置渲染效果
        // 设置chartview观察器的大小范围
//        chartview->setMinimumSize(500,500);
        chartview->setMinimumSize(350,320); // A4
//        chartview->setMaximumSize(INT_MAX,INT_MAX); // 使用默认最大值
        // 设置chartview具有放大镜功能【！！！】
        chartview->setRubberBand(QChartView::RectangleRubberBand);


    } catch (...) {
        qDebug() << "Chart::Chart";
        throw;
    }
}

void Chart::setAxis(QString _xname, qreal _xmin, qreal _xmax, int _xtickc,
             QString _yname, qreal _ymin, qreal _ymax, int _ytickc){
    /*
        说明：
            纵坐标使用固定分隔，横坐标使用自定义分隔【有空维护】【！！！】
    */
    try {
        xname = _xname; xmin = _xmin; xmax = _xmax; xtickc = _xtickc;
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
        // 设置坐标轴的描述字体和大小
        QFont font = QFont("Consolas");
//        QFont font;
        font.setStyleStrategy(QFont::PreferAntialias);
//        font.setPointSize(1);
//        font.setPointSize(11);
        font.setPixelSize(11);
        font.setBold(true);

        // 设置轴的字体大小
        QFont axis_font = QFont("Consolas");
        axis_font.setStyleStrategy(QFont::PreferAntialias);
        axis_font.setPixelSize(11);
        axisX->setLabelsFont(axis_font);
        axisY->setLabelsFont(axis_font);axisX->setLabelsVisible(true);

        // 设置X轴描述
        axisX->setGridLineVisible(true);   //x轴网格线可见
        axisX->setLabelFormat("%u");   //设置x轴的刻度标签格式
        axisX->setTitleText(xname);  //设置x轴的描述
        axisX->setTitleFont(font);
        axisX->setRange(xmin, xmax);    //设置x轴的数据范围
        axisX->setTickCount(xtickc);       //x轴设置实网格线的数量【也是一个大格】
        axisX->setMinorTickCount(1);   //设置x轴每个大格里面小刻度线的数目

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
        qDebug() << "Chart::setAxis";
        throw;
    }
}

void Chart::buildChart(const vector<int>& scatter_sites,int site_max_parts,
                       const QMap<int,QVector<QPointF>>& series_data,
                       const pair<double,double>& XI_line_data,
                       const pair<double,double>& attri_XI)
{
    /*
        参数：QVector<QVector<QPointF>> series_data:
            attri -> [site][part] = (part,val)的类型

        功能：
            对chart进行构造，其中每个site的数据线都进行构造并加入chart中，实现chart的构造

        存储内容：
            Chart类内：
                由于可能出现断点，因此需要使用该结构：QVector<QVector<QLineSeries*>>
                    横坐标，代表每个site芯片对应属性上的线，【实际就是线组】
                    纵坐标，代表这条线由一组线所构成，每条线是由多个点构成的线数据【实际就是点组】
                    row: attri -> [site]
                    col: [Line_part]s = [LineSeries] -> [Points]
    */
    try {
        // 设置chart名称
        QFont font = QFont("Consolas");
        font.setPixelSize(14);
        font.setBold(true);
        qchart->setTitleFont(font);
        qchart->setTitle(chartname);

        // 设置数据线粗细
        int data_series_width = 3;
        // 设置最值线粗细
        int XI_series_width = 3;

        // 【特殊最值处理】：如果没有数值存在时，图表什么都不画
        if(XI_line_data.first == INT_MIN && XI_line_data.second == INT_MAX)return;

        // 绘制数据线
        construct_datas_series(scatter_sites,site_max_parts,series_data,data_series_width);

        // 绘制最值线 // 【最值线的绘制，只与attri_XI有关】
        construct_XI_line(attri_XI,XI_series_width,site_max_parts);
        // 修正图例样式
        construct_legend_style(scatter_sites,attri_XI);

    } catch (...) {
        qDebug() << "Chart::buildChart";
        throw;
    }

}

void Chart::time_buildChart(const vector<string> &scatter_time_sites,int site_max_parts,
                            const QMap<string,QVector<QPointF> > & time_series_data,
                            const pair<double, double> & XI_line_data,
                            const pair<double, double> & attri_XI)
{
    try {
        // 设置chart名称
        QFont font = QFont("Consolas");
        font.setPixelSize(14);
        font.setBold(true);
        qchart->setTitleFont(font);
        qchart->setTitle(chartname);

        // 设置数据线粗细
        int data_series_width = 3;
        // 设置最值线粗细
        int XI_series_width = 3;

        // 【特殊最值处理】：如果没有数值存在时，图表什么都不画
        if(XI_line_data.first == INT_MIN && XI_line_data.second == INT_MAX)return;

        // 绘制数据线
        construct_time_datas_series(scatter_time_sites,site_max_parts,time_series_data,data_series_width);

        // 绘制最值线 // 【最值线的绘制，只与attri_XI有关】
        construct_time_XI_line(attri_XI,XI_series_width,site_max_parts);
        // 修正图例样式
        construct_time_legend_style(scatter_time_sites,attri_XI);

    } catch (...) {
        qDebug() << "Chart::time_buildChart";
        throw;
    }
}

void Chart::construct_datas_series(const vector<int>& scatter_sites,int site_max_parts,
                                   const QMap<int,QVector<QPointF>> &series_data, int data_series_width)
{
    /*
        参数：
            series_data：数据线的线组数据，其结构为[site][part] -> QPointF (其中part为线组)
            data_series_width：设置数据线的宽度
        功能：
            将数据的数据点组，转换为数据线，并添加到chart中
    */
    try {
//        qDebug() << series_data.size();
//        qDebug() << series_data[0].size();
        // 填充series中的数据
        // 遍历属性下的每组芯片
        qreal zero = 0.0;

        for(size_t i = 0;i < scatter_sites.size();i++)
        {
            int site = scatter_sites[i];
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
            for(int part = 1;part <= site_max_parts;)
            {
                // 如果不为NULL点，则点不为(0,0)，正常添加点数据
                if(series_data[site][part].x() != zero || series_data[site][part].y() != zero)
                {
                    series[site][grp]->append(series_data[site][part]);
                    series[site][grp]->setPointsVisible(); // 将具体点标注出来
//                    series[site][grp]->setPointLabelsVisible(); // 点上出现具体数值
                    part++;
                }
                // 如果为(0,0)点，则创建一个新的点组进行记录【实现分段处理】
                else {
                    // 连续的(0,0)间断，需要进行跨越
                    while(series_data[site][part].x() == zero && series_data[site][part].y() == zero)
                    {
                        // 防止连续++造成越界
                        part++;
                        if(part > site_max_parts) break;
                    }
                    // 没有数据点，不需要去构造线【当part已经越界时】
                    if( part <= site_max_parts)
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
        for(size_t i = 0;i < scatter_sites.size();i++)
        {
            int site = scatter_sites[i];
            // 这里的含义是series中的线组的part
//            for(int parts = 0;parts <= grp;parts++)
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
        qDebug() << "Chart::construct_datas_series";
        throw;
    }
}

void Chart::construct_XI_line(const pair<double,double>& attri_XI,
                              int XI_series_width,int site_max_parts)
{
    /*
        参数：
            XI_line_data：最值线的限值数据
            XI_series_width：最值线的宽度
            series_data：为了查看数据为(0,0)的点时，依然能够画他的线【只是用series不能实现】
        功能：
            实现绘制最值线
    */
    try {
        // 添加最值线【都是红色的虚线】
        // 如果最大值线本身并不存在
        if(attri_XI.second != INT_MAX)
        {
            // 最大值线
            QLineSeries* max_line = new QLineSeries(this);
            // 初始化点组上生成线时的初始化数据
            max_line->clear();
            max_line->setPen(QPen(Qt::red,XI_series_width,Qt::DashLine));

            for(int part = 1;part <= site_max_parts;part++)
            {
                max_line->append(QPointF(part,attri_XI.second));
            }

            qchart->addSeries(max_line);
            qchart->setAxisX(axisX, max_line);
            qchart->setAxisY(axisY, max_line);

        }

        // 如果最小值线，本身并不存在
        if(attri_XI.first != INT_MIN)
        {
            // 最小值线
            QLineSeries* min_line = new QLineSeries(this);
            // 初始化点组上生成线时的初始化数据
            min_line->clear();
            min_line->setPen(QPen(Qt::red,XI_series_width,Qt::DashLine));
            // 遍历每组芯片内的点数据
            for(int part = 1;part <= site_max_parts;part++)
            {
                min_line->append(QPointF(part,attri_XI.first));
            }
            // 添加LineSeries 加入chart
            qchart->addSeries(min_line);
            // 设置x轴和y轴与LineSeries的点数据进行对应
            qchart->setAxisX(axisX, min_line);
            qchart->setAxisY(axisY, min_line);

        }

    } catch (...) {
        qDebug() << "Chart::construct_XI_line";
        throw;
    }
}

void Chart::construct_legend_style(const vector<int> scatter_sites,const pair<double,double>& attri_XI)
{
    /*
        功能：
            设置chart的图例样例形式
    */
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
        font.setPixelSize(10);
        qchart->legend()->setFont(font);
        // 获取所有图例的markers，修改图里描述内容
        QList<QLegendMarker *> legends = qchart->legend()->markers();
//        qDebug() << (QLineSeries*())

        size_t legends_size = legends.size();

        // 计算最值线的存在个数
        int max_line_num = 0;
        if(attri_XI.first != INT_MIN)max_line_num++;
        if(attri_XI.second != INT_MAX)max_line_num++;

        // 设置图例中的文字描述
        // 绘制数据图例
        // 只对数据线图例进行扫描设置
        for(size_t i = 0;i < legends_size - max_line_num;i++)
        {
            // site数据线的图例
            // 线的绘制顺序，与芯片的顺序是一致的，由于绘制时都使用scatter_sites作为遍历顺序的依据
            int site = scatter_sites[i];

            // 将同一个site的多条part线跳过，只保留其中一个即可
            int scope = series[site].size();
            while(scope > 1)
            {
                legends[i]->setVisible(false);
                scope--;i++;
            }

            legends[i]->setLabel("site"+QString::fromStdString(to_string(site)));

        }

        // 绘制最值图例
        // 判断最值线的绘制图例【图例的绘制顺序与绘制最值线时有关】
        // 只有一条最值线存在时
        if(max_line_num == 1)
        {
            legends[legends_size - max_line_num]->setLabel(
                        attri_XI.second != INT_MAX ? "max_line" : "min_line");
        }
        // 两条最值线都存在时
        if(max_line_num == 2)
        {
            legends[legends_size - max_line_num]->setLabel("max_line");
            legends[legends_size - max_line_num + 1]->setLabel("min_line");
        }
        // site最值线的图例
//        legends[i]->setLabel(i == legends_size-2 ? "max_line" : "min_line");

    } catch (...) {
        qDebug() << "Chart::construct_legend_style";
        throw;
    }

}

void Chart::construct_time_datas_series(const vector<string> &scatter_time_sites,
                                        int site_max_parts,
                                        const QMap<string, QVector<QPointF> > &time_series_data,
                                        int data_series_width)
{
    try {
//        qDebug() << series_data.size();
//        qDebug() << series_data[0].size();
        // 填充series中的数据
        // 遍历属性下的每组芯片
        qreal zero = 0.0;

        for(size_t i = 0;i < scatter_time_sites.size();i++)
        {
            string time_site = scatter_time_sites[i];
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
            time_series.insert(time_site,t_line_vec);
            // 记录当前线组的序号
            int grp = 0;

            // 遍历每组芯片内的点数据
            for(int part = 1;part <= site_max_parts;)
            {
                // 如果不为NULL点，则点不为(0,0)，正常添加点数据
                if(time_series_data[time_site][part].x() != zero || time_series_data[time_site][part].y() != zero)
                {
                    time_series[time_site][grp]->append(time_series_data[time_site][part]);
                    time_series[time_site][grp]->setPointsVisible(); // 将具体点标注出来
//                    series[site][grp]->setPointLabelsVisible(); // 点上出现具体数值
                    part++;
                }
                // 如果为(0,0)点，则创建一个新的点组进行记录【实现分段处理】
                else {
                    // 连续的(0,0)间断，需要进行跨越
                    while(time_series_data[time_site][part].x() == zero &&
                          time_series_data[time_site][part].y() == zero)
                    {
                        // 防止连续++造成越界
                        part++;
                        if(part > site_max_parts) break;
                    }
                    // 没有数据点，不需要去构造线【当part已经越界时】
                    if( part <= site_max_parts)
                    {
                        // 跳过了连续的(0,0)点
                        QLineSeries* t_line = new QLineSeries(this);
                        t_line->clear();
                        t_line->setPen(QPen(this->colors[i],data_series_width,Qt::SolidLine));
                        time_series[time_site].push_back(t_line);
                        grp++;
                    }
                }
            }
        }

        // 遍历series中的每条线，将生成好的多组线加入chart中
        // 直接遍历LineSeries 加入chart即可，因为line的颜色已经区分了每组site，
        //      因此间断的line并不会影响到chart生成的线数据
        for(size_t i = 0;i < scatter_time_sites.size();i++)
        {
            string time_site = scatter_time_sites[i];
            // 这里的含义是series中的线组的part
//            for(int parts = 0;parts <= grp;parts++)
            for(int parts = 0;parts < time_series[time_site].size();parts++)
            {
                // 添加LineSeries 加入chart
                qchart->addSeries(time_series[time_site][parts]);
                // 设置x轴和y轴与LineSeries的点数据进行对应
                qchart->setAxisX(axisX, time_series[time_site][parts]);
                qchart->setAxisY(axisY, time_series[time_site][parts]);
            }
        }

    } catch (...) {
        qDebug() << "Chart::construct_time_datas_series";
        throw;
    }
}

void Chart::construct_time_XI_line(const pair<double, double> &attri_XI, int XI_series_width, int site_max_parts)
{
    try {
        construct_XI_line(attri_XI,XI_series_width,site_max_parts);

    } catch (...) {
        qDebug() << "Chart::construct_time_XI_line";
        throw;
    }
}

void Chart::construct_time_legend_style(const vector<string> scatter_time_sites,
                                        const pair<double, double> &attri_XI)
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
        font.setPixelSize(10);
        qchart->legend()->setFont(font);
        // 获取所有图例的markers，修改图里描述内容
        QList<QLegendMarker *> legends = qchart->legend()->markers();
//        qDebug() << (QLineSeries*())

        size_t legends_size = legends.size();

        // 计算最值线的存在个数
        int max_line_num = 0;
        if(attri_XI.first != INT_MIN)max_line_num++;
        if(attri_XI.second != INT_MAX)max_line_num++;

        // 设置图例中的文字描述
        // 绘制数据图例
        // 只对数据线图例进行扫描设置
        for(size_t i = 0;i < legends_size - max_line_num;i++)
        {
            // site数据线的图例
            // 线的绘制顺序，与芯片的顺序是一致的，由于绘制时都使用scatter_sites作为遍历顺序的依据
            string time_site = scatter_time_sites[i];

            // 将同一个site的多条part线跳过，只保留其中一个即可
            int scope = time_series[time_site].size();
            while(scope > 1)
            {
                legends[i]->setVisible(false);
                scope--;i++;
            }

            legends[i]->setLabel("site"+QString::fromStdString(time_site));

        }

        // 绘制最值图例
        // 判断最值线的绘制图例【图例的绘制顺序与绘制最值线时有关】
        // 只有一条最值线存在时
        if(max_line_num == 1)
        {
            legends[legends_size - max_line_num]->setLabel(
                        attri_XI.second != INT_MAX ? "max_line" : "min_line");
        }
        // 两条最值线都存在时
        if(max_line_num == 2)
        {
            legends[legends_size - max_line_num]->setLabel("max_line");
            legends[legends_size - max_line_num + 1]->setLabel("min_line");
        }
        // site最值线的图例
//        legends[i]->setLabel(i == legends_size-2 ? "max_line" : "min_line");

    } catch (...) {
        qDebug() << "Chart::construct_time_legend_style";
        throw;
    }
}








































