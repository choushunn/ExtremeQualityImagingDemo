#include "connxfacepaint.h"

COnnxFacePaint::COnnxFacePaint(const std::string &model_path, bool isGPU)
{
    env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNX");
    sessionOptions = Ort::SessionOptions();
    //初始化 cuda
    std::vector<std::string> availableProviders = Ort::GetAvailableProviders();
    auto cudaAvailable = std::find(availableProviders.begin(),
                                   availableProviders.end(),
                                   "CUDAExecutionProvider");
    OrtCUDAProviderOptions cudaOption;
    if (isGPU && (cudaAvailable == availableProviders.end()))
    {
        qDebug() << "GPU is not supported by your ONNXRuntime build. Fallback to CPU.";
        qDebug() << "Inference device: CPU";
    }
    else if (isGPU && (cudaAvailable != availableProviders.end()))
    {
        qDebug() << "Inference device: GPU" ;
        sessionOptions.AppendExecutionProvider_CUDA(cudaOption);
    }
    else
    {
        qDebug() << "Inference device: CPU" ;
    }

    //const std::string model_path1 = "./models/face_paint_512_v2_0.onnx";
    std::wstring widestr = std::wstring(model_path.begin(), model_path.end());
    session = Ort::Session(env, widestr.c_str(), sessionOptions);
    this->getModelInfo();
    qDebug() <<"000.COnnx init.";
}

void COnnxFacePaint::getModelInfo(){
    Ort::AllocatorWithDefaultOptions allocator;
    qDebug() << "input_count:"<<session.GetInputCount();
    qDebug() << "output_count:" << session.GetOutputCount();
    qDebug() << "version:" << session.GetModelMetadata().GetVersion();
    // 获取输入输出张量信息
    Ort::TypeInfo input_node_info = session.GetInputTypeInfo(0);
    Ort::TypeInfo output_node_info = session.GetOutputTypeInfo(0);

    input_tensor_size_ = input_node_info.GetTensorTypeAndShapeInfo().GetElementCount();
    output_tensor_size_ = output_node_info.GetTensorTypeAndShapeInfo().GetElementCount();

    input_node_dims_ =input_node_info.GetTensorTypeAndShapeInfo().GetShape();
    output_node_dims_ =output_node_info.GetTensorTypeAndShapeInfo().GetShape();
    qDebug() << "input_tensor_size:"<< input_tensor_size_ << output_tensor_size_;
    qDebug() << "input_node_dims:"<<input_node_dims_ << output_node_dims_;


    for(int i =0; i<session.GetInputCount();i++){
        qDebug() << i<<"input_name:" <<session.GetInputNameAllocated(0,allocator).get();
        inputNames.push_back(session.GetInputNameAllocated(i,allocator).get());
    }

    for(int i=0; i<session.GetOutputCount();i++){
        qDebug() << i<<"output_name:" <<session.GetOutputNameAllocated(0,allocator).get();
        outputNames.push_back(session.GetOutputNameAllocated(i,allocator).get());
    }
}

COnnxFacePaint::~COnnxFacePaint()
{
    session.release();
    sessionOptions.release();
    env.release();
}


void COnnxFacePaint:: preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor){

}



void COnnxFacePaint:: run(const cv::Mat input_image, cv::Mat& output_image){
    cv::Mat dst_image;
    int height = input_node_dims_[2]; //480
    int width =  input_node_dims_[3];  //640
    cv::resize(input_image, dst_image, cv::Size(width, height));
    qDebug() << "dst_image.cols=:width" << dst_image.cols <<"x" << dst_image.rows;
    //将输入的OpenCV Mat CV8UC3图像转换，1.CHW->HWC，2.转CV32FC3
    std::vector<float> input_image_f32(output_tensor_size_);
    for (int c = 0; c < dst_image.channels(); c++)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                float pix = dst_image.ptr<uchar>(i)[j * 3 + 2 - c];  //BGR2RGB
                input_image_f32[c * height * width + i * width+ j] = (pix/255.0); //转CV32FC3
            }
        }
    }

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator,
                                                            OrtMemType::OrtMemTypeDefault);

    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo,
                                                             input_image_f32.data(),
                                                             input_image_f32.size(),
                                                             input_node_dims_.data(),
                                                             input_node_dims_.size());
    assert(inputTensor.IsTensor());
    const char* input_names[]={"input_image"};
   const char* output_names[]={"output_image"};
    auto outputTensor = session.Run(Ort::RunOptions{ nullptr },
                                    input_names,
                                    &inputTensor,
                                    1,
                                    output_names,
                                    1);   // 开始推理


    //输出处理
    float* outputData = outputTensor[0].GetTensorMutableData<float>();
    //将输出的Tensor转换，1.HWC->CHW，2.CV32FC3->CV8UC3
    cv::Mat dst = dst_image.clone();
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                float pix = outputData[0]*0.5+0.5;
                pix = (pix > 0) ? pix : 0;
                pix = (pix < 1) ? pix : 1;
                pix *= 255;
                dst.ptr<uchar>(i)[j * 3 + 2 - c] = (uchar)pix; ///RGB2BGR
                outputData++;
            }
        }
    }
    cv::cvtColor(dst, output_image, cv::COLOR_BGR2RGB);
}



void COnnxFacePaint:: postProcessing(Ort::Value& output_tensor, cv::Mat& output_image){


}
