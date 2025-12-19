// ===============================
// Pipeline.cpp
// ===============================


#include "Core/Pipeline.h"
#include <opencv2/imgproc.hpp> // cv::cvtColor

void Pipeline::process(const cv::Mat& input, cv::Mat& output)
{
    // Safety check: do nothing if input is empty
    if (input.empty()) return;

    // ============================================
    // TODO: Connect Vision Module here later.
    // For now: Convert to Grayscale as a proof of concept.
    // ============================================

    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
}