#include "connx.h"
#include "qdebug.h"
#include <ostream>
void printModelInfo(Ort::Session &session, Ort::AllocatorWithDefaultOptions &allocator)
{
    //输出模型输入节点的数量
    size_t num_input_nodes = session.GetInputCount();
    size_t num_output_nodes = session.GetOutputCount();
    qDebug()<<"Number of input node is:"<<num_input_nodes;
    qDebug()<<"Number of output node is:"<<num_output_nodes;

    //获取输入输出维度
    for(auto i = 0; i<num_input_nodes;i++)
    {
        std::vector<int64_t> input_dims = session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        qDebug()<<"input "<<i<<" dim is: ";
        for(auto j=0; j<input_dims.size();j++)
            qDebug()<<input_dims[j]<<" ";
    }
    for(auto i = 0; i<num_output_nodes;i++)
    {
        std::vector<int64_t> output_dims = session.GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        qDebug()<<"output "<<i<<" dim is: ";
        for(auto j=0; j<output_dims.size();j++)
            qDebug()<<output_dims[j]<<" ";
    }
    //输入输出的节点名

    //    for(auto i = 0; i<num_input_nodes;i++)
    //      qDebug()<<"The input op-name "<<i<<" is:"<< session.GetInputNameAllocated(i, allocator);
    //    for(auto i = 0; i<num_output_nodes;i++)
    //       qDebug()<<"The output op-name "<<i<<" is:"<< session.GetInputNameAllocated(i, allocator);

    //input_dims_2[0] = input_dims_1[0] = output_dims[0] = 1;//batch size = 1
}


COnnx::COnnx(QObject *parent)
    : QObject{parent}
{
    //init env
    env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNXRUNTIME");
    sessionOptions = Ort::SessionOptions();
    std::vector<std::string> availableProviders = Ort::GetAvailableProviders();
    auto cudaAvailable = std::find(availableProviders.begin(), availableProviders.end(), "CUDAExecutionProvider");
    OrtCUDAProviderOptions cudaOption;
    bool isGPU = false;
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
//    Ort::SessionOptions session_options;
//    OrtSessionOptionsAppendExecutionProvider_CUDA(session_options, 0);
//    session_options.SetIntraOpNumThreads(4);
//    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
#ifdef _WIN32
    const wchar_t* model_path = L"model.onnx";
#else
    const char* model_path = "/home/mc/Projects/runtime/Net.onnx";
#endif
    //    Ort::Session session(env,model_path,session_options);
    //    Ort::AllocatorWithDefaultOptions allocator;
    //    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator,OrtMemType::OrtMemTypeDefault);
    //    //打印模型的信息
    //    printModelInfo(session,allocator);
}

void run(){
    //    preProcess();
    //session.run()
}


void COnnx::preProcessing(cv::Mat &image){

}


void COnnx::postProcessing(cv::Mat &image){

}
