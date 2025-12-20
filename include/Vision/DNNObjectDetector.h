#pragma once

#include "Vision/IVisionAlgorithm.h" 
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>



class DNNObjectDetector : public IVisionAlgorithm
{
public:
   
    bool init(const std::string& modelPath, const std::string& configPath, const std::string& classesPath);


    void detect(const cv::Mat& input, cv::Mat& output) override;
    const std::vector<cv::Rect>& getLatestDetections() const { return m_latestDetections; }

private:
    std::vector<std::string> getOutputNames(const cv::dnn::Net& net);
    void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);
    cv::dnn::Net m_net;
    std::vector<std::string> m_classNames;
    std::vector<cv::Rect> m_latestDetections;
};