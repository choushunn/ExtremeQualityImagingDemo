#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>
#include <QEvent>
#include <qdebug.h>
#include "utils.h"


// 自定义事件类型
enum MyEventType {
    FlipEvent = QEvent::User + 1,
    GrayEvent,
    ScaleEvent,
    HistEvent,
    rgbHistEvent,
};


class AppEvent : public QObject
{
    Q_OBJECT

public:
    explicit AppEvent(QObject *parent = nullptr);
    QVector<MyEventType> m_eventQueue; //动态数组容器


public slots:
    void processFrame(cv::Mat frame);

signals:
    void sendProcessFrame(cv::Mat image);
};


#endif // APPEVENT_H
