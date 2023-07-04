#ifndef APPINIT_H
#define APPINIT_H

#include <QMainWindow>
#include <QMediaDevices>
#include <QCamera>
#include <toupcam.h>
#include "ctoupcamera.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "connx.h"
#include "ccamera.h"
#include "cthread.h"

namespace Ui{
class MainWindow;
}

class AppInit : public QMainWindow
{
    Q_OBJECT
public:
    AppInit();
    explicit AppInit(Ui::MainWindow *ui);
    COnnx* onnx= nullptr;
//    CCamera* camera = nullptr;
    CThread* m_cthread = nullptr;
    // 将CCamera指针类型转换为CToupCamera指针类型
//    CToupCamera* toup = dynamic_cast<CToupCamera*>(camera);
    CToupCamera* camera = nullptr;

private:
    Ui::MainWindow       *mainwindowUi;
    QList<QCameraDevice> m_cameraList;
    int m_cameraIndex = 0;
    std::string camera_type = "USB";


private:
    void initMainWindowUI();
    void initCamera();
    void initOnnx();

signals:

};

#endif // APPINIT_H
