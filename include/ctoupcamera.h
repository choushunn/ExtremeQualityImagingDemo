#ifndef CTOUPCAMERA_H
#define CTOUPCAMERA_H

#include <toupcam.h>
#include "ccamera.h"
#include "qdebug.h"


class CToupCamera : public CCamera
{

public:
    CToupCamera(int index);
    ~CToupCamera();
    bool isOpened() const override;
    int open() override;
    void close() override;
    bool read(cv::Mat& frame) override;
    void getCameraList(std::vector<std::string> &camera_list);
    virtual void setAutoExposure(bool state) override;
    virtual void setExpoTime(int value) override;
    virtual Context createContext() override {
        Context context;
        return context;
    }
    Context context_;
    HToupcam        m_hcam = nullptr;

private:
    int m_index;
    uchar*          m_pData = nullptr;
    int             m_res;
    unsigned        m_imgWidth;
    unsigned        m_imgHeight;
    ToupcamFrameInfoV2* pInfo;
    ToupcamDeviceV2 m_arr[TOUPCAM_MAX]; //所有相机
    unsigned toupCamCount;

private:
    void evtCallback(unsigned nEvent);
    static void __stdcall eventCallBack(unsigned nEvent, void *pCallbackCtx);
    void handleStillImageEvent();
};

#endif // CTOUPCAMERA_H
