#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    ~MyLabel();
    void select();
    void rectangle(int code);
    void paintEvent(QPaintEvent *ev);
    cv::Rect* crop();
    QImage original;
    cv::Mat current;
    QPoint point1;
    QPoint point2;
    bool selection;
signals:
private:


public slots:
};

#endif // MYLABEL_H
