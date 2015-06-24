#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include "Qpainter.h"
#include <QtWidgets>
#include <unistd.h>
#include <string.h>
using namespace std;
char * buf;
string str(getcwd(buf,200));
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    str=str+"\\g.jpg";
    ui->setupUi(this);
    ui->imageLabel->installEventFilter(this);   //installing the eventfilter
    ui->openButton->installEventFilter(this);
    ui->reset->installEventFilter(this);
    ui->crop->installEventFilter(this);
    ui->zoomIn->installEventFilter(this);
    ui->zoomOut->installEventFilter(this);
    ui->slider->installEventFilter(this);
    ui->reset->setEnabled(false);
    ui->crop->setEnabled(false);
    ui->zoomIn->setEnabled(false);
    ui->zoomOut->setEnabled(false);
    ui->slider->setEnabled(false);
    ui->slider->setRange(0,360);    
    ui->imageLabel->setScaledContents(true);
}
bool MainWindow::eventFilter(QObject *sender, QEvent *event)
{
    if (sender == ui->openButton){
        if(event->type() == QEvent::MouseButtonPress){
            QStringList mimeTypeFilters;
            foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
                mimeTypeFilters.append(mimeTypeName);
            mimeTypeFilters.sort();
            const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
            QFileDialog dialog(this, tr("Open File"),
                               picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
            dialog.setAcceptMode(QFileDialog::AcceptOpen);
            dialog.setMimeTypeFilters(mimeTypeFilters);
            dialog.selectMimeTypeFilter("image/jpeg");
            while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
        }
    }
    else if(sender == ui->zoomIn && ui->zoomIn->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            scaleImage(1.25);
        }
    }
    else if(sender == ui->zoomOut && ui->zoomOut->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            scaleImage(0.8);
        }
    }
    else if(sender == ui->crop && ui->crop->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            cv::Rect * temp=ui->imageLabel->crop();
            ui->imageLabel->current=ui->imageLabel->current(*temp);
            cv::imwrite(str,ui->imageLabel->current);
            loadFile(QString(str.c_str()));

        }
    }
    else if(sender == ui->reset && ui->reset->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->setPixmap(QPixmap::fromImage(ui->imageLabel->original));
            ui->imageLabel->adjustSize();
            cv::Mat tmp(ui->imageLabel->original.height(),ui->imageLabel->original.width(),CV_8UC3,(uchar*)ui->imageLabel->original.bits(),ui->imageLabel->original.bytesPerLine());
            cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
            cv::cvtColor(tmp, result,CV_BGR2RGB);
            ui->imageLabel->current=result;
        }
    }
    else if(sender == ui->slider && ui->reset->isEnabled()){
        if(event->type() == QEvent::MouseMove){
                double angle=90;
                // get rotation matrix for rotating the image around its center
                cv::Point2f center(ui->imageLabel->current.cols/2.0, ui->imageLabel->current.rows/2.0);
                cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
                // determine bounding rectangle
                cv::Rect bbox = cv::RotatedRect(center,ui->imageLabel->current.size(), angle).boundingRect();
                // adjust transformation matrix
                rot.at<double>(0,2) += bbox.width/2.0 - center.x;
                rot.at<double>(1,2) += bbox.height/2.0 - center.y;
                cv::warpAffine(ui->imageLabel->current, ui->imageLabel->current, rot, bbox.size(),cv::INTER_CUBIC);
                cv::imwrite(str,ui->imageLabel->current);
                loadFile(QString(str.c_str()));
        }
    }
    return QWidget::eventFilter(sender,event);
}
bool MainWindow::loadFile(const QString &fileName)
{
    QImage image(fileName);    
    ui->imageLabel->current=cv::imread(fileName.toStdString(), 1);
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        ui->imageLabel->setPixmap(QPixmap());
        ui->imageLabel->adjustSize();
        return false;
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;
    ui->imageLabel->original=image;
    ui->zoomIn->setEnabled(true);
    ui->zoomOut->setEnabled(true);
    ui->crop->setEnabled(true);
    ui->reset->setEnabled(true);
    ui->slider->setEnabled(true);
    ui->imageLabel->adjustSize();
    setWindowFilePath(fileName);
    return true;
}
void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= factor;
    cv::resize(ui->imageLabel->current,ui->imageLabel->current,cv::Size(),factor,factor,cv::INTER_CUBIC);
    cv::imwrite(str,ui->imageLabel->current);
    loadFile(QString(str.c_str()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
