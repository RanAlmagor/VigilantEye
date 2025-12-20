#include "HAL/Sensors/VideoFileSource.h"
#include "GeneralUtils/Logger.h"

#include <utility>
#include <opencv2/videoio.hpp>

VideoFileSource::VideoFileSource(std::string filePath, int targetFps, bool loop)
    : m_filePath(std::move(filePath))
    , m_sourceName("VideoFile_" + m_filePath)
    , m_targetFps(targetFps)
    , m_loop(loop)
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

    m_cap.open(m_filePath);

    if (!m_cap.isOpened())
    {
        Logger::getInstance().log("[ERROR] Failed to open video file: " + m_filePath);
        return false;
    }

   
    if (m_targetFps > 0)
        m_cap.set(cv::CAP_PROP_FPS, m_targetFps);

    m_width = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    m_height = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    double fps = m_cap.get(cv::CAP_PROP_FPS);
    if (fps >= 1.0 && fps <= 240.0) {
        m_fps = fps;
    }
    else {
        m_fps = (m_targetFps > 0 ? (double)m_targetFps : 30.0);
        Logger::getInstance().log("[HAL] VideoFileSource: FPS not reported, using fallback: " + std::to_string(m_fps));
    }

    Logger::getInstance().log(
        "[HAL] Initialized " + m_sourceName +
        " | Actual: " + std::to_string(m_width) + "x" + std::to_string(m_height) +
        " | FPS: " + std::to_string(m_fps) +
        " | Loop: " + std::string(m_loop ? "ON" : "OFF")
    );

    return true;
}

bool VideoFileSource::rewindToStart()
{
    if (!m_cap.isOpened())
        return false;

    bool ok = m_cap.set(cv::CAP_PROP_POS_FRAMES, 0);
    if (!ok) {
        ok = m_cap.set(cv::CAP_PROP_POS_MSEC, 0);
    }
    return ok;
}

bool VideoFileSource::capture(cv::Mat& frame)
{
    if (!m_cap.isOpened())
        return false;

    if (m_cap.read(frame) && !frame.empty())
        return true;

    if (!m_loop)
        return false;

    if (!rewindToStart()) {
        Logger::getInstance().log("[HAL] VideoFileSource: rewind failed.");
        return false;
    }

    if (!m_cap.read(frame) || frame.empty()) {
        Logger::getInstance().log("[HAL] VideoFileSource: read failed after rewind.");
        return false;
    }

    return true;
}

void VideoFileSource::stop() noexcept
{
    if (m_cap.isOpened())
        m_cap.release();
}
