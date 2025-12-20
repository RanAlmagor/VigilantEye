#pragma once

#include "Vision/IVisionAlgorithm.h" 
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct Detection
{
    int classId;
    float confidence;
    cv::Rect box;
    std::string label;
};

class DNNObjectDetector : public IVisionAlgorithm
{
public:
    
    DNNObjectDetector() = default;

    
    ~DNNObjectDetector() = default;

    //  === חסימת העתקה (Rule of 5) ===
 
    DNNObjectDetector(const DNNObjectDetector&) = delete;           
    DNNObjectDetector& operator=(const DNNObjectDetector&) = delete; 

    // 4.  (Move Semantics) ===
  
    DNNObjectDetector(DNNObjectDetector&&) = default;
    DNNObjectDetector& operator=(DNNObjectDetector&&) = default;

    
    bool init(const std::string& modelPath, const std::string& configPath, const std::string& classesPath);
    void detect(const cv::Mat& input, cv::Mat& output) override;
    const std::vector<Detection>& getLatestDetections() const { return m_latestDetections; }

private:
    cv::dnn::Net m_net;
    std::vector<std::string> m_classNames;
    std::vector<Detection> m_latestDetections;
};