#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/logo.ico"));
    appInit  = new AppInit(ui);
    appEvent = new AppEvent(this);
    //appEvent线程
    //    appEventThread = new QThread();
    //    appEvent->moveToThread(appEventThread);
    //    appEventThread->start();
    m_timer  = new QTimer();
//    m_searchTimer = new QTimer();
//    m_searchTimer->setInterval(120);
//    connect(m_searchTimer, &QTimer::timeout, this, &MainWindow::on_timeoutSearch);
    qDebug() << "MainWindow" <<QThread::currentThreadId() << QThread::currentThread();
    //系统自带的QStyle风格
    QStringList listStyle = QStyleFactory::keys();
    //打印当前系统支持的系统风格
    foreach(QString val, listStyle){
        ui->comboBox_2->addItem(val);
        qDebug()<<val<<"  ";
    }
    qApp->setStyle(QStyleFactory::create("Fusion"));
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
//            connect(appInit->webCamera, &CUSBCamera::sendFrame, appEvent, &AppEvent::processFrame);
//            connect(appInit->webCamera, &CUSBCamera::sendFrame, appInit->ncnnYolo, &CNcnn::detect);
            //显示帧
//            connect(appInit->webCamera, &CUSBCamera::sendFrame, this, &MainWindow::showFrame);
//            connect(appEvent, &AppEvent::sendProcessFrame, this, &MainWindow::showFrame);
            connect(appInit->webCamera, &CUSBCamera::sendFrame, appInit->connx, &COnnx::run);
            connect(appInit->connx, &COnnx::sendFrame, this, &MainWindow::showFrame);
        }
        else if(ui->m_cbx_camera_type->currentText() == "TOUP")
        {
            //打开摄像头
            appInit->toupCamera->open();
            m_timer->start();
            //读取帧
            connect(m_timer, &QTimer::timeout, appInit->toupCamera, &CToupCamera::read);
            qDebug() <<"";
            //处理帧
//            connect(appInit->toupCamera, &CToupCamera::sendFrame, appInit->ncnnYolo, &CNcnn::detect);

            connect(appInit->toupCamera, &CToupCamera::sendFrame, appInit->connx, &COnnx::run);
            connect(appInit->connx, &COnnx::sendFrame, this, &MainWindow::showFrame);
            //显示帧
//            connect(appInit->toupCamera, &CToupCamera::sendFrame, this, &MainWindow::showFrame);
        }
        ui->m_cbx_camera_list->setDisabled(true);
        ui->m_cbx_camera_type->setDisabled(true);
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
        ui->m_cbx_camera_list->setDisabled(false);
        ui->m_cbx_camera_type->setDisabled(false);
    }
}

/**
 * @brief 显示QImage
 * @param image    接收到的QImage
 */
void MainWindow::showFrame(cv::Mat frame)
{
        qDebug() << "MainWindow:3.show frame.";
    cv::resize(frame, frame, cv::Size(ui->m_lbl_display1->height(), ui->m_lbl_display1->width()));
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

void MainWindow::on_pushButton_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(GrayEvent)){
            appEvent->m_eventQueue.append(GrayEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(GrayEvent)){
            appEvent->m_eventQueue.removeAll(GrayEvent);
        }
    }
}


void MainWindow::on_action_fullscreen_triggered()
{
    if(MainWindow::isFullScreen()){
        MainWindow::showNormal();
    }else{
        MainWindow::showFullScreen();
    }
}


void MainWindow::on_action_close_triggered()
{
    int ret = QMessageBox::warning(this, "退出", "是否退出程序", QMessageBox::Ok, QMessageBox::Cancel);
    switch(ret)
    {
    case QMessageBox::Ok:
        qDebug() <<"退出程序";
        QApplication::quit();
        break;
    case QMessageBox::Cancel:
        qDebug() <<"取消退出程序";
        break;
    default:
        break;
    }
}


void MainWindow::on_action_maxscreen_triggered()
{
    if(MainWindow::isMaximized()){
        MainWindow::showNormal();
    }else{
        MainWindow::showMaximized();
    }
}




void MainWindow::on_action_normal_triggered()
{
    MainWindow::showNormal();

}


void MainWindow::on_action_open_triggered()
{
        QString path = QDir::currentPath();
        QString fileName = QFileDialog::getOpenFileName(this, "选择一个文件", path,  "图像文件 (*.jpg *.png *.bmp)");

        // 如果选择了文件，则加载图像文件并显示在标签控件中
        if (!fileName.isEmpty())
        {
            QPixmap pixmap(fileName);
            ui->m_lbl_display1->setPixmap(pixmap);
        }
}


void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    //设置当前风格为
    qApp->setStyle(QStyleFactory::create(ui->comboBox_2->currentText()));
}

