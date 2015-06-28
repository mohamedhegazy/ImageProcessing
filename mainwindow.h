#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollBar>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <string.h>
#include <opencv2/imgproc/imgproc.hpp>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool loadFile(const QString &);
    void scaleImage(double factor);
    void write();
    void display(std::string temp);
    cv::Mat read();
    void adjustScrollBar(QScrollBar *scrollBar, double factor);    
    ~MainWindow();
protected:
    bool eventFilter( QObject* sender, QEvent* event);
private:
    Ui::MainWindow *ui;
    double scaleFactor;
};

#endif // MAINWINDOW_H
