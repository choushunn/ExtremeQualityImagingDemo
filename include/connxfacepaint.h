#ifndef CONNXFACEPAINT_H
#define CONNXFACEPAINT_H
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <onnxruntime_cxx_api.h>

#include "connxbase.h"

class COnnxFacePaint:public COnnxBase
{
public:
    COnnxFacePaint(const std::string &model_path="./models/face_paint_512_v2_0", bool isGPU=false);
    ~COnnxFacePaint();
    void run(const cv::Mat input_image, cv::Mat& output_image) override ;

private:
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor)override ;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image)override ;
    void getModelInfo();
private:
    Ort::Env env{nullptr};
    Ort::SessionOptions sessionOptions{nullptr};
    Ort::Session session{nullptr};
    std::vector<const char*> inputNames{nullptr};
    std::vector<const char*> outputNames{nullptr};
    // 获取输入输出维度
    std::vector<int64_t> input_node_dims_;
    std::vector<int64_t> output_node_dims_;
    // 获取输入输出张量大小
    size_t input_tensor_size_;
    size_t output_tensor_size_;
};

#endif // CONNXFACEPAINT_H
