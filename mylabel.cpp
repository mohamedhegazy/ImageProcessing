#include "mylabel.h"
#include <QtWidgets>
MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{

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
void MyLabel::mousePressEvent(QMouseEvent * event)
{
    QMouseEvent* ev=(QMouseEvent *)event;
    point1 = ev->globalPos();
    update();
}
void MyLabel::mouseReleaseEvent(QMouseEvent * event)
{
    QMouseEvent* ev=(QMouseEvent *)event;
    point2 = ev->globalPos();
    update();
}
void MyLabel::mouseMoveEvent(QMouseEvent *event){
    QMouseEvent* ev=(QMouseEvent *)event;
    point2 = ev->globalPos();
    update();
}
void MyLabel::paintEvent(QPaintEvent *ev){
    QLabel::paintEvent(ev);
    QPainter painter(this);
    if(!point1.isNull() && !point2.isNull()){
        int topX=point1.x()<point2.x()?point1.x():point2.x();
        int topY=point1.y()<point2.y()?point1.y():point2.y();
        int width=point1.x()<point2.x()?point2.x()-point1.x():point1.x()-point2.x();
        int height=point1.y()<point2.y()?point2.y()-point1.y():point1.y()-point2.y();
        painter.drawRect(topX,topY,width,height);
    }
}

