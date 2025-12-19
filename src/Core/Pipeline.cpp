#include "Core/Pipeline.h"

void Pipeline::process(const cv::Mat& input, cv::Mat& output)
{
    // Pass-through (no copy when possible).
    // If you plan to draw overlays, you may want output = input.clone();
    output = input;
}
