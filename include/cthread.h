#ifndef CTHREAD_H
#define CTHREAD_H

#include <QThread>
#include "ccamera.h"
#include "connxbase.h"

class CThread : public QThread
{
    Q_OBJECT
public:
    explicit CThread(QObject *parent = nullptr);
    ~CThread();
    void setCamera(CCamera* camera);
    void setCONNX(COnnxBase* connx);
    void stop();
signals:
    void newFrame(const cv::Mat& frame);
    void inferenceFinished(const cv::Mat& output);
protected:
    void run() override;
private:
    CCamera* m_camera;
    COnnxBase* m_connx;
    bool m_stop;
signals:

};

#endif // CTHREAD_H
