// ===============================
// VideoFileSource.cpp
// ===============================
#include "HAL/Sensors/VideoFileSource.h" 
#include "GeneralUtils/Logger.h"

#include <utility> // std::move

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
        Logger::getInstance().log("[ERROR] VideoFileSource: empty file path.");
        return false;
    }

    Logger::getInstance().log("[HAL] Opening " + m_sourceName + " | Path: " + m_filePath + "...");

    // Open file
    m_cap.open(m_filePath);

    if (!m_cap.isOpened())
    {
        Logger::getInstance().log("[ERROR] Failed to open video file: " + m_filePath);
        return false;
    }

    // Best-effort request
    if (m_targetFps > 0)
        m_cap.set(cv::CAP_PROP_FPS, m_targetFps);

    // Read actual properties
    m_width = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    m_height = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));


    m_fps = m_cap.get(cv::CAP_PROP_FPS);


    std::string logMsg = "[HAL] Initialized " + m_sourceName +
        " | Actual: " + std::to_string(m_width) + "x" + std::to_string(m_height) +
        " | FPS: " + std::to_string(m_fps);
    Logger::getInstance().log(logMsg);

    return true;
}

bool VideoFileSource::capture(cv::Mat& frame)
{
    if (!m_cap.isOpened())
    {
        return false;
    }

    // For files, read() returns false at end-of-stream
    if (!m_cap.read(frame))
    {
        return false;
    }

    if (frame.empty())
    {
        return false;
    }

    return true;
}

void VideoFileSource::stop() noexcept
{
    if (m_cap.isOpened())
    {
        m_cap.release();
    }
}