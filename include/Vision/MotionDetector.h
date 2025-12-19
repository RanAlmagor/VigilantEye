#ifndef VISION_MOTION_DETECTOR_H
#define VISION_MOTION_DETECTOR_H

#include <opencv2/core.hpp>

class MotionDetector
{
public:
    MotionDetector() = default;

    /**
     * @brief Detects motion by comparing the current frame to the previous one.
     * @param input The current frame captured from the camera.
     * @param mask Output: a binary image (white = motion detected).
     */
    void detect(const cv::Mat& input, cv::Mat& mask);

private:
    cv::Mat m_prevFrame; // Stores the previous frame for motion comparison
};

#endif // VISION_MOTION_DETECTOR_H
