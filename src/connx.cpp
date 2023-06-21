#include "connx.h"



COnnx::COnnx(QObject *parent)
    : QObject{parent}
{
    //init env
    env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNXRUNTIME");
    sessionOptions = Ort::SessionOptions();

    //初始化 cuda
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


//初始化 model
#ifdef _WIN32
    const wchar_t* model_path = L"./models/Net.onnx";
#else
    const char* model_path = "/home/mc/runtime/models/Net.onnx";
#endif

    //初始化 session
    session =  Ort::Session(env, model_path, sessionOptions);

    Ort::AllocatorWithDefaultOptions allocator;
    Ort::TypeInfo inputTypeInfo = session.GetInputTypeInfo(0);
    std::vector<int64_t> inputTensorShape = inputTypeInfo.GetTensorTypeAndShapeInfo().GetShape();
    this->isDynamicInputShape = false;
    if(inputTensorShape[2] == -1 && inputTensorShape[3] == -1){
        qDebug() <<"Dynamic input shape";
        this->isDynamicInputShape = true;
    }
    qDebug() << "Input shape:" <<inputTensorShape;

    //shuru/shuchu canshu
    QString inputName = session.GetInputNameAllocated(0, allocator).get();
    QString outputName = session.GetOutputNameAllocated(0, allocator).get();

    qDebug() << "Input name:" << inputName;
    qDebug() << "Output name:" << outputName;
    //
    //    this->inputImageShape = cv::Size2f(inputSize);
}

void COnnx::preProcessing(cv::Mat &image, float*& blob, std::vector<int64_t>& inputTensorShape,cv::Mat &floatImage){

    //    cv::Mat floatImage;
    image.convertTo(floatImage, CV_32FC3, 1.0/255);
    //    cv::transpose(floatImage,floatImage);
    //    cv::cvtColor(image, resizedImage, cv::COLOR_BGR2RGB);
    inputTensorShape[2] = floatImage.rows;
    inputTensorShape[3] = floatImage.cols;

    //    resizedImage.convertTo(floatImage, CV_32FC3, 1/255.0);
    blob = new float[floatImage.cols*floatImage.rows*floatImage.channels()];
    cv::Size floatImageSize{floatImage.cols, floatImage.rows};
    //hwc->chw
    std::vector<cv::Mat> chw(floatImage.channels());
    for(int i=0; i<floatImage.channels(); ++i)
    {
        chw[i] =cv::Mat(floatImageSize, CV_32FC1, blob+i*floatImageSize.width*floatImageSize.height);
    }
    cv::split(floatImage, chw);

}

size_t vectorProduct(const std::vector<int64_t>& vector){
    if(vector.empty()){
        return 0;
    }
    size_t product = 1;
    for(const auto & element:vector){
        product *= element;
    }
    return product;
}

void COnnx::run(cv::Mat image){
    float *blob = nullptr;
    std::vector<int64_t> inputTensorShape{1, 3, -1, -1};
    cv::Mat floatImage;
    this->preProcessing(image, blob, inputTensorShape, floatImage);
    size_t inputTensorSize = vectorProduct(inputTensorShape);
    qDebug() << "inputTensorSize:" << inputTensorSize;
    std::vector<float> inputTensorValues(blob, blob+inputTensorSize);
    std::vector<Ort::Value> inputTensors;

    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);


    const char* input_names[] ={"actual_input_1"};
    const char* output_names[] = {"output1"};
    inputTensors.push_back(Ort::Value::CreateTensor<float>(memory_info,
                                                           inputTensorValues.data(),
                                                           inputTensorValues.size(),
                                                           inputTensorShape.data(),
                                                           inputTensorShape.size()));

    std::vector<Ort::Value> outputTensors = this->session.Run(Ort::RunOptions{nullptr},
                                                              input_names,
                                                              inputTensors.data(),
                                                              1,
                                                              output_names,
                                                              1);


    std::vector<int64_t> outputShape = outputTensors[0].GetTensorTypeAndShapeInfo().GetShape();
    qDebug() << "outputShape:"<<outputShape;
    float* outputData = outputTensors[0].GetTensorMutableData<float>();
    cv::Mat output_image(outputShape[2], outputShape[3], CV_32FC3, outputData);
    cv::cvtColor(output_image, output_image, cv::COLOR_RGB2BGR);
    //    output_image *= 255;
    output_image.convertTo(output_image, CV_8UC3, 255);
    emit sendFrame(output_image);
    cv::imwrite("output.png", output_image);
    //    const QString win = "img";
    //    cv::imshow(WIN, output_image);
    //    cv::waitKey();
    //    cv::destroyAllWindows();
    delete[] blob;
}





//void COnnx::postProcessing(cv::Mat &image){

//}
