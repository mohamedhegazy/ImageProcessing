#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include "Qpainter.h"
#include <QtWidgets>
#include <unistd.h>
using namespace std;
QString original_path;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    ui->imageLabel->installEventFilter(this);   //installing the eventfilter
    ui->openButton->installEventFilter(this);
    ui->reset->installEventFilter(this);
    ui->crop->installEventFilter(this);
    ui->zoomIn->installEventFilter(this);
    ui->zoomOut->installEventFilter(this);
    ui->slider->installEventFilter(this);
    ui->select->installEventFilter(this);
    ui->left->installEventFilter(this);
    ui->down->installEventFilter(this);
    ui->up->installEventFilter(this);
    ui->right->installEventFilter(this);
    ui->save->installEventFilter(this);
    ui->save->setEnabled(false);
    ui->left->setEnabled(false);
    ui->down->setEnabled(false);
    ui->up->setEnabled(false);
    ui->right->setEnabled(false);
    ui->reset->setEnabled(false);
    ui->crop->setEnabled(false);
    ui->zoomIn->setEnabled(false);
    ui->zoomOut->setEnabled(false);
    ui->slider->setEnabled(false);
    ui->select->setEnabled(false);
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
            original_path=dialog.selectedFiles().first();
            ui->imageLabel->current=cv::imread(dialog.selectedFiles().first().toStdString(), 1);
            ui->imageLabel->angle=0;
            ui->slider->setValue(0);
            scaleFactor = 1.0;
        }
    }
    else if(sender == ui->zoomIn && ui->zoomIn->isEnabled()){        
        if(event->type() == QEvent::MouseButtonPress){
            scaleImage(1.25);
            deselect();
        }
    }
    else if(sender == ui->zoomOut && ui->zoomOut->isEnabled()){        
        if(event->type() == QEvent::MouseButtonPress){
            scaleImage(0.8);
            deselect();
        }
    }
    else if(sender == ui->crop && ui->crop->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress && ui->imageLabel->selection && !ui->imageLabel->point1.isNull() && !ui->imageLabel->point2.isNull()){
            cv::Rect * temp=ui->imageLabel->crop();            
            cv::resize(ui->imageLabel->current,ui->imageLabel->current,cv::Size(),scaleFactor,scaleFactor,cv::INTER_CUBIC);
            double angle=ui->imageLabel->angle;
            cv::Point2f center(ui->imageLabel->current.cols/2.0, ui->imageLabel->current.rows/2.0);
            cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
            // determine bounding rectangle
            cv::Rect bbox = cv::RotatedRect(center,ui->imageLabel->current.size(), angle).boundingRect();
            // adjust transformation matrix
            rot.at<double>(0,2) += bbox.width/2.0 - center.x;
            rot.at<double>(1,2) += bbox.height/2.0 - center.y;
            cv::warpAffine(ui->imageLabel->current, ui->imageLabel->current, rot, bbox.size(),cv::INTER_CUBIC,cv::BORDER_CONSTANT,cv::Scalar(255,255,255));
            ui->imageLabel->current=ui->imageLabel->current(*temp);
            ui->imageLabel->angle=0;
            ui->slider->setValue(0);
            scaleFactor=1.0;
            display("xxxxxxxxxxxxxxxxxxxxxx.jpg");
            deselect();
        }
    }
    else if(sender == ui->reset && ui->reset->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->angle=0;
            ui->slider->setValue(0);
            scaleFactor=1.0;
            ui->imageLabel->current=cv::imread(original_path.toStdString(), 1);
            loadFile(original_path);
            display("xxxxxxxxxxxxxxxxxxxxxx.jpg");
            deselect();
        }
    }
    else if(sender == ui->slider && ui->slider->isEnabled()){
        if(event->type() == QEvent::MouseMove){
            ui->imageLabel->angle=ui->slider->value();
            display("xxxxxxxxxxxxxxxxxxxxxx.jpg");
            deselect();
        }
    }
    else if(sender == ui->select && ui->select->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->select();
            ui->left->setEnabled(true);
            ui->down->setEnabled(true);
            ui->up->setEnabled(true);
            ui->right->setEnabled(true);
        }
    }
    else if(sender == ui->left && ui->left->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->rectangle(Qt::Key_Left);
        }
    }
    else if(sender == ui->right && ui->right->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->rectangle(Qt::Key_Right);
        }
    }
    else if(sender == ui->up && ui->up->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->rectangle(Qt::Key_Up);
        }
    }
    else if(sender == ui->down && ui->down->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            ui->imageLabel->rectangle(Qt::Key_Down);
        }
    }
    else if(sender == ui->save && ui->save->isEnabled()){
        if(event->type() == QEvent::MouseButtonPress){
            QStringList mimeTypeFilters;
            foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
                mimeTypeFilters.append(mimeTypeName);
            mimeTypeFilters.sort();
            const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
            QFileDialog dialog(this, tr("Save File"),
                               picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            dialog.setMimeTypeFilters(mimeTypeFilters);
            dialog.selectMimeTypeFilter("image/jpeg");
            if(dialog.exec() == QDialog::Accepted) {display(dialog.selectedFiles().first().toStdString());}
        }
    }
    return QWidget::eventFilter(sender,event);
}
bool MainWindow::loadFile(const QString &fileName)
{    
    QImage image(fileName);    
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        ui->imageLabel->setPixmap(QPixmap());
        ui->imageLabel->adjustSize();
        return false;
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));      
    ui->save->setEnabled(true);
    ui->zoomIn->setEnabled(true);
    ui->zoomOut->setEnabled(true);
    ui->crop->setEnabled(true);
    ui->reset->setEnabled(true);
    ui->slider->setEnabled(true);
    ui->select->setEnabled(true);
    ui->imageLabel->adjustSize();
    setWindowFilePath(fileName);
    return true;
}
void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= factor;
    display("xxxxxxxxxxxxxxxxxxxxxx.jpg");
}
void MainWindow::display(string temp1){
    cv::Mat temp;
    cv::resize(ui->imageLabel->current,temp,cv::Size(),scaleFactor,scaleFactor,cv::INTER_CUBIC);
    double angle=ui->imageLabel->angle;
    cv::Point2f center(temp.cols/2.0, temp.rows/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle
    cv::Rect bbox = cv::RotatedRect(center,temp.size(), angle).boundingRect();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - center.x;
    rot.at<double>(1,2) += bbox.height/2.0 - center.y;  
    cv::warpAffine(temp, temp, rot, bbox.size(),cv::INTER_CUBIC,cv::BORDER_CONSTANT,cv::Scalar(255,255,255));
    cv::imwrite(temp1,temp);
    if(strcmp(temp1.c_str(),"xxxxxxxxxxxxxxxxxxxxxx.jpg")==0)
    loadFile(QString(temp1.c_str()));
}
void MainWindow::deselect(){
    ui->imageLabel->selection=false;
    ui->imageLabel->point1.setX(0);
    ui->imageLabel->point1.setY(0);
    ui->imageLabel->point2.setX(0);
    ui->imageLabel->point2.setY(0);
    ui->left->setEnabled(false);
    ui->down->setEnabled(false);
    ui->up->setEnabled(false);
    ui->right->setEnabled(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
