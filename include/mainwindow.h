#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyleFactory>
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

    void on_action_close_triggered();

    void on_action_fullscreen_triggered();

    void on_action_normal_triggered();

    void on_action_open_triggered();

    void on_action_maxscreen_triggered();

    void on_comboBox_2_currentIndexChanged(int index);

    void readFrame() ;


private:

    AppInit*    appInit;
//    AppEvent&   appEvent =AppEvent::getInstance();
    QThread*    appThread;
    QTimer*     m_timer = nullptr;
    int         fps = 30;

};
#endif // MAINWINDOW_H
