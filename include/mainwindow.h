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
    void on_pushButton_clicked();

private:

    AppInit*    appInit;
    QThread*    appThread;
    QTimer*     m_timer = nullptr;
    int         fps = 30;
private slots:
    void onInferenceFinished(const cv::Mat& output);
    void on_m_slider_expoTime_valueChanged(int value);
    void on_m_cbox_auto_stateChanged(int arg1);
    void on_m_slider_expoGain_valueChanged(int value);
    void on_action_triggered();
};
#endif // MAINWINDOW_H
