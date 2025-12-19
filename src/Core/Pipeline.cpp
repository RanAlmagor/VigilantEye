#include "Core/Pipeline.h"

void Pipeline::addAlgorithm(std::unique_ptr<IVisionAlgorithm> algo)
{
    m_algorithms.push_back(std::move(algo));
}

void Pipeline::process(const cv::Mat& input, cv::Mat& output)
{
    if (input.empty()) return;

  
    input.copyTo(output);

    
    for (const auto& algo : m_algorithms) {
        algo->detect(input, output);
    }
}