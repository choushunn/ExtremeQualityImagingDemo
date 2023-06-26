#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/logo.ico"));
    appInit  = new AppInit(ui);
    m_timer  = new QTimer();
    //系统自带的QStyle风格
    QStringList listStyle = QStyleFactory::keys();
    //打印当前系统支持的系统风格
    foreach(QString val, listStyle){
        ui->comboBox_2->addItem(val);
        qDebug()<<val<<"  ";
    }
    qApp->setStyle(QStyleFactory::create("Fusion"));
//    connect(appInit->m_cthread, &CThread::newFrame, this, &MainWindow::onNewFrame);
//    connect(appInit->m_cthread, &CThread::inferenceFinished, this, &MainWindow::onInferenceFinished);

}



MainWindow::~MainWindow()
{
    delete ui;
}

//读取图像槽函数
void MainWindow::readFrame() {
    cv::Mat image;
    appInit->camera->read(image);
    if (!image.empty()) {
        this->showFrame(image);
    }
}


/**
 * @brief 打开相机信号槽
 * @param
 */
void MainWindow::on_m_btn_open_camera_clicked(bool checked)
{
    if(checked){
        ui->m_btn_open_camera->setText("关闭");
        m_timer->setInterval(int(1000/fps));
        appInit->camera->open();
        m_timer->start();
        connect(m_timer, &QTimer::timeout, this, &MainWindow::readFrame);
        ui->m_cbx_camera_list->setDisabled(true);
        ui->m_cbx_camera_type->setDisabled(true);
//        appInit->m_cthread->start();
    }else{
                appInit->camera->close();
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
    if(frame.empty()){

        return;
    }
    cv::resize(frame, frame, cv::Size(640, 480));

    QSize size = ui->m_lbl_display1->size();
    QImage qimage1(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    QPixmap pixmap1 = QPixmap::fromImage(qimage1);
    pixmap1 = pixmap1.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display1->setPixmap(pixmap1);

    if(ui->m_btn_load_algorithm->isChecked()){
        // 初始化输出图像
        cv::Mat output_image;
        appInit->onnx->run(frame, output_image);
        cv::resize(output_image, output_image, cv::Size(640, 480));
        QImage qimage(output_image.data, output_image.cols, output_image.rows, QImage::Format_RGB888);
        QPixmap pixmap = QPixmap::fromImage(qimage);
        pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);
        ui->m_lbl_display2->setPixmap(pixmap);
    }
    else{
        ui->m_lbl_display2->clear();
    }
}



void MainWindow::on_action_fullscreen_triggered()
{
    //设置全屏
    if(MainWindow::isFullScreen()){
        MainWindow::showNormal();
    }else{
        MainWindow::showFullScreen();
    }
}


void MainWindow::on_action_close_triggered()
{
    //退出程序
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
    //最大化窗口
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

void MainWindow::on_pushButton_clicked()
{
    // 打开图像文件对话框
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open Image"), "C:\\Users\\Spring\\Pictures\\images", tr("Image Files (*.png *.jpg *.bmp)"));

    // 读取图像文件
    cv::Mat image = cv::imread(file_path.toStdString());

    //文件为空
    if (image.empty()) {
        return;
    }else{
        // 初始化输出图像
        cv::resize(image, image, cv::Size(640, 480));
        //显示输入图像
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    }
    // 调整图像大小以适应控件大小
    QSize size = ui->m_lbl_display1->size();
    cv::Mat output_image;
    if(ui->m_btn_load_algorithm->isChecked()){
        appInit->onnx->run(image, output_image);
        //结果图
        cv::resize(output_image, output_image, cv::Size(640, 480));
        // 将图像转换为Qt格式
        QImage qimage1(output_image.data, output_image.cols, output_image.rows, QImage::Format_RGB888);
        qDebug() << output_image.cols << output_image.rows;
        QPixmap pixmap1 = QPixmap::fromImage(qimage1);
        // 调整图像大小以适应控件大小
        //    QSize size1 = ui->label_2->size();
        pixmap1 = pixmap1.scaled(size, Qt::KeepAspectRatio);
        // 将图像显示到控件中
        ui->m_lbl_display2->setPixmap(pixmap1);
        ui->m_lbl_display2->setAlignment(Qt::AlignCenter);
    }


    // 将图像转换为Qt格式
    QImage qimage(image.data, image.cols, image.rows, QImage::Format_RGB888);
    qDebug() << image.cols << image.rows;
    QPixmap pixmap = QPixmap::fromImage(qimage);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);
    // 将图像显示到控件中
    ui->m_lbl_display1->setPixmap(pixmap);
    ui->m_lbl_display1->setAlignment(Qt::AlignCenter);
}

void MainWindow::onInferenceFinished(const cv::Mat& output){
    showFrame(output);
}
