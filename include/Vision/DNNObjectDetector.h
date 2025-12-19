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

private:
    cv::dnn::Net m_net;
    std::vector<std::string> m_classNames;
};