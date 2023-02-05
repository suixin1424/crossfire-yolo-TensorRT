#include <cuda_runtime_api.h>
#include <iostream>
#include "NvInfer.h"
#include "NvInferPlugin.h"
#include "NvInferRuntimeCommon.h"
#include "NvOnnxParser.h"
#include <opencv2/opencv.hpp>

#include <math.h>
#include <array>
#include <fstream>

#include <random>
#include <sstream>
#include <string>
#include <vector>

using nvinfer1::Dims2;
using nvinfer1::Dims3;
using nvinfer1::IBuilder;
using nvinfer1::IBuilderConfig;
using nvinfer1::ICudaEngine;
using nvinfer1::IExecutionContext;
using nvinfer1::IHostMemory;
using nvinfer1::ILogger;
using nvinfer1::INetworkDefinition;
using Severity = nvinfer1::ILogger::Severity;

using cv::Mat;
using std::array;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;
using std::vector;


class Logger : public ILogger {
 public:
  void log(Severity severity, const char* msg) noexcept override {
    if (severity != Severity::kINFO) {
      std::cout << msg << std::endl;
    }
  }
};
class Yolo {
public:
    Yolo(char* model_path);
    float letterbox(
        const cv::Mat& image,
        cv::Mat& out_image,
        const cv::Size& new_shape,
        int stride,
        const cv::Scalar& color,
        bool fixed_shape,
        bool scale_up);
    float* blobFromImage(cv::Mat& img);
    void draw_objects(const cv::Mat& img, float* Boxes, int* ClassIndexs, int* BboxNum);
    void Init(char* model_path);
    void Infer(
        int aWidth,
        int aHeight,
        int aChannel,
        unsigned char* aBytes,
        float* Boxes,
        int* ClassIndexs,
        int* BboxNum);
    ~Yolo();

private:
    nvinfer1::ICudaEngine* engine = nullptr;
    nvinfer1::IRuntime* runtime = nullptr;
    nvinfer1::IExecutionContext* context = nullptr;
    cudaStream_t stream = nullptr;
    void* buffs[5];
    int iH, iW, in_size, out_size1, out_size2, out_size3, out_size4;
    Logger gLogger;
};
