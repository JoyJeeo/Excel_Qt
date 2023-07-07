#include "chart.h"

Chart::Chart(QWidget* parent, QString _chartname){
    setParent(parent);
    chartname = _chartname; // 初始化chart名称
//    series1 = new QSplineSeries(this);
//    series2 = new QSplineSeries(this);
//    series3 = new QSplineSeries(this);
//    series4 = new QSplineSeries(this);
    // 先不初始化所有线

    qchart = new QChart;
    chartview = new QChartView(qchart); // 将chart图表添加入chart显示器中
    layout = new QHBoxLayout(this); // 添加对象到布局中
    axisX = new QValueAxis(this);
    axisY = new QValueAxis(this);
    //在ui里面添加了一个Widget并把曲线图添加进去
    layout->addWidget(chartview); // chart显示器添加入布局中
//    setLayout(layout); // 将布局容器放入最大的widget布局容器中
    chartview->setRenderHint(QPainter::Antialiasing);//防止图形走样：抗锯齿 // 设置渲染效果
}

void Chart::setAxis(QString _xname, qreal _xmin, qreal _xmax, int _xtickc,
             QString _yname, qreal _ymin, qreal _ymax, int _ytickc){
    xname = _xname; xmin = _xmin; xmax = _xmax; xtickc = _xtickc;
    yname = _yname; ymin = _ymin; ymax = _ymax; ytickc = _ytickc;

    axisX->setRange(xmin, xmax);    //设置x轴的数据范围
    axisX->setLabelFormat("%u");   //设置x轴的刻度标签格式
    /************************************
        %u 无符号十进制整数
        %s 字符串
        %c 一个字符
        %d 有符号十进制整数
        %e 浮点数、e-记数法
        %f 浮点数、十进制记数法
        %s 字符串
    ****************************************/
        axisX->setGridLineVisible(true);   //x轴网格线可见
        axisX->setTickCount(xtickc);       //x轴设置实网格线的数量【也是一个大格】
        axisX->setMinorTickCount(1);   //设置x轴每个大格里面小刻度线的数目
        axisX->setTitleText(xname);  //设置x轴的描述
        axisY->setRange(ymin, ymax); // 设置y轴的数据范围
        axisY->setLabelFormat("%u"); // 设置y轴的刻度标签格式
        axisY->setGridLineVisible(true); // y轴网格线可见
        axisY->setTickCount(ytickc); // y轴设置实网格线的数量【也是一个大格】
        axisY->setMinorTickCount(1); // 设置y轴每个大格里面小刻度线的数目
        axisY->setTitleText(yname); // 设置y轴的描述
        // 将数轴添加入chart图表中
        qchart->addAxis(axisX, Qt::AlignBottom); //下：Qt::AlignBottom  上：Qt::AlignTop
        qchart->addAxis(axisY, Qt::AlignLeft);   //左：Qt::AlignLeft    右：Qt::AlignRight
}

void Chart::buildChart(QList<QPointF> pointlist)
{
    /*根据复数的QList<QList<QPointF>>，在chart中同时绘制复数的线，并加入chart*/
    int counter = 0;
    series.push_back(QVector<QLineSeries*>());
    series[0].push_back(new QLineSeries(this));
    series[0][counter]->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    series[0][counter]->clear();
    for(int i=0; i<pointlist.size();i++)
        if(pointlist[i].x() == 0 && pointlist[i].y() == 0)
        {
            counter++;
            series[0].push_back(new QLineSeries(this));
            series[0][counter]->setPen(QPen(Qt::blue,2,Qt::SolidLine));
            series[0][counter]->clear();
        }
        else {
            series[0][counter]->append(pointlist[i].x(), pointlist[i].y());
        }

//    int colors[4]={Qt::darkBlue,Qt::darkRed,Qt::gray,Qt::darkYellow};
//    for(int i = 0;i < 4;i++)
//    {
//        series[i]->setPen(QPen(Qt::GlobalColor(colors[i]),2,Qt::SolidLine));
//        series[i]->clear();
//        for(int i=0; i<pointlist.size();i++)
//            series[i]->append(pointlist[i].x(), pointlist[i].y());

//        qchart->setTitle(chartname);
//        qchart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线动画模式
//        qchart->legend()->hide(); //隐藏图例【隐藏每条图线的具体描述】
//        qchart->addSeries(series[i]);//输入数据【添加series曲线数据】
//        // 设置x轴和y轴与series数据进行对应
//        qchart->setAxisX(axisX, series[i]);
//        qchart->setAxisY(axisY, series[i]);
//    }
    //创建数据源
//    series->setPen(QPen(Qt::blue,3,Qt::SolidLine));
//    series->clear();
//    for(int i=0; i<pointlist.size();i++)
//        series->append(pointlist[i].x(), pointlist[i].y());

//    qchart->setTitle(chartname);
//    qchart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线动画模式
//    qchart->legend()->hide(); //隐藏图例【隐藏每条图线的具体描述】
//    qchart->addSeries(series);//输入数据【添加series曲线数据】
//    // 设置x轴和y轴与series数据进行对应
//    qchart->setAxisX(axisX, series);
//    qchart->setAxisY(axisY, series);

}

