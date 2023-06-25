#ifndef APPINIT_H
#define APPINIT_H

#include <QMainWindow>
#include <QMediaDevices>
#include <QCamera>
#include <toupcam.h>
#include "ui_mainwindow.h"
#include "utils.h"
#include "connxbase.h"
#include "ccamera.h"

namespace Ui{
class MainWindow;
}

class AppInit : public QMainWindow
{
    Q_OBJECT
public:
    AppInit();
    explicit AppInit(Ui::MainWindow *ui);
    COnnxBase* onnx= nullptr;
    CCamera* camera = nullptr;


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
