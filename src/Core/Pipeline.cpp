#include "Core/Pipeline.h"

void Pipeline::addAlgorithm(std::unique_ptr<IVisionAlgorithm> algo)
{
    // Move ownership to the vector
    m_algorithms.push_back(std::move(algo));
}

void Pipeline::process(const cv::Mat& input, cv::Mat& output)
{
    if (input.empty()) return;

    // Start with a clean copy
    input.copyTo(output);

    // Run all algorithms in sequence
    for (const auto& algo : m_algorithms) {
        algo->detect(input, output);
    }
}