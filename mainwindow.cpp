#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/logo.ico"));
    appInit  = new AppInit(ui);
//    appEvent = new AppEvent(this);
    //appEvent线程
    //    appEventThread = new QThread();
    //    appEvent->moveToThread(appEventThread);
    //    appEventThread->start();
    m_timer  = new QTimer();
//    m_searchTimer = new QTimer();
//    m_searchTimer->setInterval(120);
//    connect(m_searchTimer, &QTimer::timeout, this, &MainWindow::on_timeoutSearch);
    qDebug() << "MainWindow" <<QThread::currentThreadId() << QThread::currentThread();
}



MainWindow::~MainWindow()
{
    delete ui;
}





/**
 * @brief 打开相机信号槽
 * @param
 */
void MainWindow::on_m_btn_open_camera_clicked(bool checked)
{
    if(checked){
        ui->m_btn_open_camera->setText("关闭");
        //1s读10帧
//        fps = ui->lineEdit_FPS->text().toInt();
        m_timer->setInterval(int(1000/fps));
//        connect(appInit->ncnnYolo, &CNcnn::sendDectectImage, this, &::MainWindow::showFrame);
        if(ui->m_cbx_camera_type->currentText() == "USB"){
            appInit->webCamera->open();
            m_timer->start();
            //读取帧
            connect(m_timer, &QTimer::timeout, appInit->webCamera, &CUSBCamera::read);
            //处理帧
            //connect(app_init->web_camera, &CUSBCamera::sendFrame, appEvent, &AppEvent::processFrame);
//            connect(appInit->webCamera, &CUSBCamera::sendFrame, appInit->ncnnYolo, &CNcnn::detect);
            //显示帧
            connect(appInit->webCamera, &CUSBCamera::sendFrame, this, &MainWindow::showFrame);
//            connect(appEvent, &AppEvent::sendProcessFrame, this, &MainWindow::showFrame);
        }
        else if(ui->m_cbx_camera_type->currentText() == "TOUP")
        {
            //打开摄像头
            appInit->toupCamera->open();
            m_timer->start();
            //读取帧
            connect(m_timer, &QTimer::timeout, appInit->toupCamera, &CToupCamera::read);
            //处理帧
//            connect(appInit->toupCamera, &CToupCamera::sendFrame, appInit->ncnnYolo, &CNcnn::detect);
            //显示帧
            connect(appInit->toupCamera, &CToupCamera::sendFrame, this, &MainWindow::showFrame);
        }
    }
    else
    {
        if(ui->m_cbx_camera_type->currentText()=="USB"){
            m_timer->stop();
            appInit->webCamera->close();
        }else if(ui->m_cbx_camera_type->currentText()=="TOUP")
        {
            appInit->toupCamera->close();
        }
        ui->m_btn_open_camera->setText("打开");
        ui->m_lbl_display1->clear();
        ui->m_lbl_display2->clear();
    }
}

/**
 * @brief 显示QImage
 * @param image    接收到的QImage
 */
void MainWindow::showFrame(cv::Mat frame)
{
    //    qDebug() << "MainWindow:3.show frame.";
//    cv::resize(frame,frame,cv::Size(ui->m_lbl_display1->height(),ui->m_lbl_display1->width()));
//    QImage new_image = image.scaled(ui->m_lbl_display1->width(), ui->m_lbl_display1->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    ui->m_lbl_display1->setPixmap(QPixmap::fromImage(cvMatToQImage(frame)));
    ui->m_lbl_display2->setPixmap(QPixmap::fromImage(cvMatToQImage(frame)));
}

///**
// * @brief 显示QImage
// * @param image    接收到的QImage
// */
//void MainWindow::showFrame(QImage image)
//{
//    //    qDebug() << "MainWindow:3.show frame.";
//    QImage new_image = image.scaled(ui->m_lbl_display1->width(), ui->m_lbl_display1->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
//    ui->m_lbl_display1->setPixmap(QPixmap::fromImage(new_image));
//    ui->m_lbl_display2->setPixmap(QPixmap::fromImage(new_image));
//}
