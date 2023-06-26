#ifndef CONNXBASE_H
#define CONNXBASE_H
#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>

class COnnx
{
public:
    COnnx();
    virtual void run(const cv::Mat input_image, cv::Mat& output_image) = 0;
    //工厂方法
    static COnnx* createInstance(const std::string& type, const std::string& model_path = "", bool isGPU = false);
private:
    virtual void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) = 0;
    virtual void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) = 0;

};

#endif // CONNXBASE_H
