#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class OpenCVDemo {
public:
    void demo1()
    {
        cv::Mat image = cv::Mat::zeros(400, 600, CV_8UC3);
        image.setTo(cv::Scalar(255, 255, 255));
        cv::fillConvexPoly(image, std::vector<cv::Point>{ cv::Point(200, 300), cv::Point(400, 300), cv::Point(300, 50) }, cv::Scalar(128, 128, 128));
        cv::fillConvexPoly(image, std::vector<cv::Point>{ cv::Point(200, 300), cv::Point(300, 175), cv::Point(300, 50) }, cv::Scalar(255, 0, 0));
        cv::fillConvexPoly(image, std::vector<cv::Point>{ cv::Point(400, 300), cv::Point(305, 255), cv::Point(305, 50) }, cv::Scalar(0, 0, 255));
        cv::fillConvexPoly(image, std::vector<cv::Point>{ cv::Point(205, 300), cv::Point(260, 240), cv::Point(395, 300) }, cv::Scalar(0, 255, 0));
        cv::imshow("CMake Trianges!!", image);
        cv::waitKey(0);
    }
};
