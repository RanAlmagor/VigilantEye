// ===============================
// VideoFileSource.cpp
// ===============================
#include "HAL/Sensors/VideoFileSource.h"

#include <iostream>
#include <utility>  // std::move

VideoFileSource::VideoFileSource(std::string filePath, int targetFps)
    : m_filePath(std::move(filePath)),
    m_sourceName("VideoFile"),
    m_targetFps(targetFps)
{
}

bool VideoFileSource::initialize()
{
    stop();

    if (m_filePath.empty())
    {
        std::cerr << "[ERROR] VideoFileSource: empty file path.\n";
        return false;
    }

    std::cout << "[HAL] Opening " << m_sourceName << " | Path: " << m_filePath << "...\n";

    // Open file
    m_cap.open(m_filePath);

    if (!m_cap.isOpened())
    {
        std::cerr << "[ERROR] Failed to open video file: " << m_filePath << "\n";
        return false;
    }

    // Best-effort request (often ignored for files, but harmless)
    if (m_targetFps > 0)
        m_cap.set(cv::CAP_PROP_FPS, m_targetFps);

    // Read actual properties
    m_width = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    m_height = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    m_fps = static_cast<int>(m_cap.get(cv::CAP_PROP_FPS));

    std::cout << "[HAL] Initialized " << m_sourceName
        << " | Actual: " << m_width << "x" << m_height
        << " | FPS: " << m_fps
        << std::endl;

    return true;
}

bool VideoFileSource::capture(cv::Mat& frame)
{
    if (!m_cap.isOpened())
        return false;

    // For files, read() returns false at end-of-stream
    if (!m_cap.read(frame))
        return false;

    if (frame.empty())
        return false;

    return true;
}

void VideoFileSource::stop() noexcept
{
    if (m_cap.isOpened())
        m_cap.release();
}
