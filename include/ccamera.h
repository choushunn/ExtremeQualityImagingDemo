#ifndef CCAMERA_H
#define CCAMERA_H
#include <opencv2/opencv.hpp>

struct Context {
    unsigned uimax;
    unsigned uimin;
    unsigned uidef;
    unsigned short usmax;
    unsigned short usmin;
    unsigned short usdef;
};

class CCamera
{
public:
    CCamera();
    virtual ~CCamera() {}
    virtual bool isOpened() const = 0;
    virtual int open() = 0;
    virtual void close() = 0;
    virtual bool read(cv::Mat& frame) = 0;
    virtual void setAutoExposure(bool state){};
    virtual void setExpoTime(int value){};
    virtual Context createContext(){
        Context context;
        return context;
    }
    //工厂方法
    static CCamera* createInstance(const std::string type, const int index);
};

#endif // CCAMERA_H
