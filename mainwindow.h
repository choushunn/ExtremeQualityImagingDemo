#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "appinit.h"
#include "appevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

private slots:
    void on_m_btn_open_camera_clicked(bool checked);
    void showFrame(cv::Mat frame);

private:

    AppInit*    appInit;
    AppEvent*   appEvent;
    QThread*    appThread;
    QTimer*     m_timer = nullptr;
    int         fps = 30;

};
#endif // MAINWINDOW_H
