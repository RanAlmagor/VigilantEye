#ifndef CORE_PIPELINE_H
#define CORE_PIPELINE_H

#include <opencv2/core.hpp>
#include <vector>
#include <memory>
#include "Vision/IVisionAlgorithm.h" 

class Pipeline
{
public:
  
    void addAlgorithm(std::unique_ptr<IVisionAlgorithm> algo);

    void process(const cv::Mat& input, cv::Mat& output);

private:
   
    std::vector<std::unique_ptr<IVisionAlgorithm>> m_algorithms;
};

#endif // CORE_PIPELINE_H