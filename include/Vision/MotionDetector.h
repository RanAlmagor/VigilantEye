#ifndef VISION_MOTION_DETECTOR_H
#define VISION_MOTION_DETECTOR_H

#include "IVisionAlgorithm.h" 

class MotionDetector : public IVisionAlgorithm
{
public:
    MotionDetector() = default;


    void detect(const cv::Mat& input, cv::Mat& output) override;

private:
    cv::Mat m_prevFrame;
};

#endif // VISION_MOTION_DETECTOR_H