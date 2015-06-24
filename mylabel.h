#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    ~MyLabel();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *ev);
    cv::Rect* crop();
    QImage original;
    cv::Mat current;
    QPoint point1;
    QPoint point2;

signals:
private:


public slots:
};

#endif // MYLABEL_H
