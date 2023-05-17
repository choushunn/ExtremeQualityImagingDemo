#ifndef CONNX_H
#define CONNX_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <onnxruntime_cxx_api.h>
#include <opencv2/highgui.hpp>
class COnnx : public QObject
{
    Q_OBJECT
public:
    explicit COnnx(QObject *parent = nullptr);
    void run(cv::Mat &image);

private:
    void preProcessing(cv::Mat &image, float*& blob, std::vector<int64_t>& inputTensorShape,cv::Mat &floatImage);
    void postProcessing(cv::Mat &image);


private:
    Ort::Env env{nullptr};
    Ort::SessionOptions sessionOptions{nullptr};
    Ort::Session session{nullptr};
    std::vector<const char*> inputNames;
    std::vector<const char*> outputNames;
//    QString inputNames;
//    QString outputNames;
    bool isDynamicInputShape{};
    cv::Size2f inputImageShape;

signals:

};

#endif // CONNX_H
