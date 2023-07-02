#include <iostream>
#include <string>
// #include "OpenCVDemo.hpp"
#include "Demo-Yolo/Yolo.h"

int main()
{
    std::cout << "Main Function Start!" << std::endl;
    cv::Mat img = cv::imread("bus.jpg", 1);
    if (img.empty()) {
        std::cout << "img is empty." << std::endl;
    }
    cv::imshow("src", img);
    cv::waitKey(2000);

    Yolo yoloDemo(NetConfig(0.5, 0.4, 608, 608, "coco.names", "yolov4/yolov4.cfg", "yolov4/yolov4.weights", "yolov4"));
    yoloDemo.DetectImg(img);
    const std::string windowName = "CNN Object detection in OpenCV";
    cv::imshow(windowName, img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
