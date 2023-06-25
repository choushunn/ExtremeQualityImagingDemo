#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>
#include <qdebug.h>




class AppEvent : public QObject
{
    Q_OBJECT

public:
    static AppEvent& getInstance();  // 获取单例对象
    void sendEvent(QObject* receiver, QEvent* event);  // 发送事件
private:
    AppEvent(QObject* parent = nullptr);  // 构造函数
    ~AppEvent();  // 析构函数
    Q_DISABLE_COPY(AppEvent)  // 禁止拷贝和赋值


signals:
    void eventReceived(QObject* receiver, QEvent* event);  // 事件接收信号
};


#endif // APPEVENT_H
