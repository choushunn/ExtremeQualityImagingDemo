#include "cthread.h"

CThread::CThread(QObject *parent)
    : QThread(parent), m_camera(nullptr), m_connx(nullptr), m_stop(false)
{

}
void CThread::setCamera(CCamera* camera)
{
    m_camera = camera;
}

void CThread::setCONNX(COnnx* connx)
{
    m_connx = connx;
}

void CThread::stop()
{
    m_stop = true;
}

CThread::~CThread()
{
    stop();
    wait();
}


void CThread::run()
{
    cv::Mat frame, output;
    while (!m_stop) {
        if (m_camera && m_camera->read(frame)) {
            m_connx->run(frame, output);
            emit inferenceFinished(output);
        }
    }
}
