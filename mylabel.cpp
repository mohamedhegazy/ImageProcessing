#include "mylabel.h"
#include <QtWidgets>
#include <QColor>
#include <QPen>
MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
selection=false;
angle=0.0;
}

MyLabel::~MyLabel()
{

}
cv::Rect * MyLabel::crop(){
    if(!point1.isNull()&& !point2.isNull()){
        int topX=point1.x()<point2.x()?point1.x():point2.x();
        int topY=point1.y()<point2.y()?point1.y():point2.y();
        int width=point1.x()<point2.x()?point2.x()-point1.x():point1.x()-point2.x();
        int height=point1.y()<point2.y()?point2.y()-point1.y():point1.y()-point2.y();
        cv::Rect* myROI=new cv::Rect(topX, topY, width, height);
        return myROI;
    }    
}
void MyLabel::select(){
    selection=true;
    point1.setX(1);
    point1.setY(1);
    point2.setX(pixmap()->width());
    point2.setY(pixmap()->height());
}
void MyLabel::rectangle(int code){
    if(selection){
        if(code==Qt::Key_Up){
            if(point2.y()-15>0)
                point2.setY(point2.y()-15);
        }
        else if(code==Qt::Key_Down){
            if(point1.y()+15<pixmap()->height())
                point1.setY(point1.y()+15);
        }
        else if(code==Qt::Key_Right){
            if(point1.x()+15<pixmap()->width())
                point1.setX(point1.x()+15);
        }
        else if(code==Qt::Key_Left){
            if(point2.x()-15>0)
                point2.setX(point2.x()-15);
        }
        update();
    }
}
void MyLabel::paintEvent(QPaintEvent *ev){
    QLabel::paintEvent(ev);
    QPainter painter(this);
    if(!point1.isNull() && !point2.isNull() && selection){
        int topX=point1.x()<point2.x()?point1.x():point2.x();
        int topY=point1.y()<point2.y()?point1.y():point2.y();
        int width=point1.x()<point2.x()?point2.x()-point1.x():point1.x()-point2.x();
        int height=point1.y()<point2.y()?point2.y()-point1.y():point1.y()-point2.y();
        QBrush brush;
        QPen pen;
        brush.setColor(Qt::red);
        pen.setStyle(Qt::DashDotDotLine);
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(topX,topY,width,height);

    }
}

