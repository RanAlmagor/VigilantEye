#ifndef VISION_I_VISION_ALGORITHM_H
#define VISION_I_VISION_ALGORITHM_H

#include <opencv2/core.hpp>


class IVisionAlgorithm
{
public:
   
    virtual ~IVisionAlgorithm() = default;

    virtual void detect(const cv::Mat& input, cv::Mat& output) = 0;
};

#endif 
