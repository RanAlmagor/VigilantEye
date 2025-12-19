// ===============================
// Pipeline.h
// ===============================

#ifndef CORE_PIPELINE_H
#define CORE_PIPELINE_H

#include <opencv2/core/mat.hpp>

class Pipeline
{
public:
    Pipeline() = default;

    /**
     * @brief Main processing method.
     * @param input  The raw frame from the sensor (Read-Only).
     * @param output The processed frame to be displayed (Write-Only).
     */
    void process(const cv::Mat& input, cv::Mat& output);
};

#endif // CORE_PIPELINE_H