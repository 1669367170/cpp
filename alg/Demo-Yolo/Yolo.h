#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

struct NetConfig {
    NetConfig(float confThresh, float nmsThresh, int32_t inputImageWidth, int32_t inputImageHeight,
        std::string classesFile, std::string modelConfig, std::string modelWeights, std::string netName)
        : confThresh(confThresh), nmsThresh(nmsThresh), inputImageWidth(inputImageWidth), inputImageHeight(inputImageHeight),
        classesFile(classesFile), modelConfig(modelConfig), modelWeights(modelWeights), netName(netName)
    {}

    float confThresh;
    float nmsThresh;
    int32_t inputImageWidth; // Width of network's input image
    int32_t inputImageHeight; // Height of network's input image
    std::string classesFile;
    std::string modelConfig;
    std::string modelWeights;
    std::string netName;
};

class Yolo {
public:
    Yolo(const NetConfig& netConfig);
    void DetectImg(cv::Mat& img);

private:
    void PostProcess(cv::Mat& img, const std::vector<cv::Mat>& outs);
    void DrawPred(int32_t classId, float conf, int32_t left, int32_t top, int32_t right, int32_t bottom, cv::Mat& img);

    float confThresh;
    float nmsThresh;
    int32_t inputImageWidth;
    int32_t inputImageHeight;

    char netName[20];
    std::vector<std::string> classes;
    cv::dnn::Net net;
};

// NetConfig yolo_nets[4] = {
//     {0,5, 0.4, 416, 416, "coco.names", "yolov3/yolov3.cfg", "yolov3/yolov3.weights", "yolov3"},
//     {0.5, 0.4, 608, 608,"coco.names", "yolov4/yolov4.cfg", "yolov4/yolov4.weights", "yolov4"},
//     {0.5, 0.4, 320, 320,"coco.names", "yolo-fastest/yolo-fastest-xl.cfg", "yolo-fastest/yolo-fastest-xl.weights", "yolo-fastest"},
//     {0.5, 0.4, 320, 320,"coco.names", "yolobile/csdarknet53s-panet-spp.cfg", "yolobile/yolobile.weights", "yolobile"}
// };
