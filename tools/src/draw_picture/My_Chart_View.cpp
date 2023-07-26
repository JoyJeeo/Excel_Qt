#include "tools/include/draw_picture/My_Chart_View.h"

My_Chart_View::My_Chart_View(QWidget* parent)
    :QChartView(parent),is_Pressed_(false)
{

}

My_Chart_View::My_Chart_View(QChart *chart, QWidget *parent)
    :QChartView(chart,parent),is_Pressed_(false)
{

}

void My_Chart_View::mouseMoveEvent(QMouseEvent *event)
{
    is_Pressed_ = false;
    QChartView::mouseMoveEvent(event);
}

void My_Chart_View::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(is_Pressed_){
            is_Pressed_ = false;
            // 单击鼠标恢复缩放
//            emit sgl_recoverRange(this);
        }
    }
    QChartView::mouseReleaseEvent(event);
}

void My_Chart_View::mousePressEvent(QMouseEvent *event)
{
    is_Pressed_ = true;
    QChartView::mousePressEvent(event);
}
