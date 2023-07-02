#include "Yolo.h"
#include <fstream>
// #include <sstream>

Yolo::Yolo(const NetConfig& netConfig) :
    confThresh(netConfig.confThresh), nmsThresh(netConfig.nmsThresh), inputImageWidth(netConfig.inputImageWidth),
    inputImageHeight(netConfig.inputImageHeight)
{
    std::cout << "Use Network: " << netConfig.netName << std::endl;
    std::string netInfo = cv::format("confThresh: %.1f, confThresh: %.1f, inputImageWidth: % d, inputImageHeight: % d",
        this->confThresh, this->nmsThresh, this->inputImageWidth, this->inputImageHeight);
    std::cout << netInfo << std::endl;

    std::ifstream ifs(netConfig.classesFile.c_str());
    std::string line;
    while (getline(ifs, line)) {
        this->classes.emplace_back(line);
    }
    this->net = cv::dnn::readNetFromDarknet(netConfig.modelConfig, netConfig.modelWeights);
    // GPU加速：cv::dnn::DNN_BACKEND_CUDA；cv::dnn::DNN_TARGET_CUDA
    this->net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    this->net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

void Yolo::DetectImg(cv::Mat& img)
{
    cv::Mat blob;
    cv::dnn::blobFromImage(img, blob, 1 / 255.0, cv::Size(this->inputImageWidth, this->inputImageHeight), cv::Scalar(0, 0, 0), true, false);
    this->net.setInput(blob);
    std::vector<cv::Mat> outs;
    this->net.forward(outs, this->net.getUnconnectedOutLayersNames());
    this->PostProcess(img, outs);

    std::vector<double> layersTimes;
    double freq = cv::getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    std::string label = cv::format("%s Inference Time: %.2f ms", this->netName, t);
    cv::putText(img, label, cv::Point(0, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
    cv::imwrite(cv::format("%s_out.jpg", this->netName), img);
}

void Yolo::PostProcess(cv::Mat& img, const std::vector<cv::Mat>& outs) // Remove the bounding boxes with low confidence using non-maxima suppression
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > this->confThresh)
            {
                int centerX = (int)(data[0] * img.cols);
                int centerY = (int)(data[1] * img.rows);
                int width = (int)(data[2] * img.cols);
                int height = (int)(data[3] * img.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, this->confThresh, this->nmsThresh, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        this->DrawPred(classIds[idx], confidences[idx], box.x, box.y,
            box.x + box.width, box.y + box.height, img);
    }
}

void Yolo::DrawPred(int32_t classId, float conf, int32_t left, int32_t top, int32_t right, int32_t bottom, cv::Mat& img)   // Draw the predicted bounding box
{
    //Draw a rectangle displaying the bounding box
    cv::rectangle(img, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 0, 255), 3);

    //Get the label for the class name and its confidence
    std::string label = cv::format("%.2f", conf);
    if (!this->classes.empty())
    {
        CV_Assert(classId < (int)this->classes.size());
        label = this->classes[classId] + ":" + label;
    }

    //Display the label at the top of the bounding box
    int baseLine;
    cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    //rectangle(img, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
    putText(img, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 255, 0), 2);
}
