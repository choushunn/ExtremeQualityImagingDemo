#ifndef CONNX_H
#define CONNX_H

#include <QObject>
#include <opencv2/core.hpp>
#include <onnxruntime_cxx_api.h>

class COnnx : public QObject
{
    Q_OBJECT
public:
    explicit COnnx(QObject *parent = nullptr);

    void preProcessing(cv::Mat &image);


    void postProcessing(cv::Mat &image);
private:
    Ort::Env env{nullptr};
    Ort::SessionOptions sessionOptions{nullptr};
    Ort::Session session{nullptr};
signals:

};

#endif // CONNX_H
