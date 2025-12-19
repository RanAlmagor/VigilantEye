// ===============================
// VideoFileSource.h
// ===============================
#ifndef HAL_SENSORS_VIDEOFILE_SOURCE_H
#define HAL_SENSORS_VIDEOFILE_SOURCE_H

#include "IFrameSource.h"

#include <opencv2/videoio.hpp>
#include <string>

class VideoFileSource : public IFrameSource
{
public:
    static constexpr int kDefaultTargetFps = 30; // best-effort (may be ignored for files)

    explicit VideoFileSource(std::string filePath,
        int targetFps = kDefaultTargetFps);

    // Rule of Five
    VideoFileSource(const VideoFileSource&) = delete;
    VideoFileSource& operator=(const VideoFileSource&) = delete;
    VideoFileSource(VideoFileSource&&) noexcept = default;
    VideoFileSource& operator=(VideoFileSource&&) noexcept = default;

    // IFrameSource
    bool initialize() override;
    bool capture(cv::Mat& frame) override;
    void stop() noexcept override;
    const std::string& getSourceName() const override { return m_sourceName; }

    ~VideoFileSource() override { stop(); }

    // Getters
    int width()  const noexcept { return m_width; }
    int height() const noexcept { return m_height; }
    int fps()    const noexcept { return m_fps; }

    const std::string& path() const noexcept { return m_filePath; }

private:
    cv::VideoCapture m_cap;

    std::string m_filePath;
    std::string m_sourceName;

    int m_targetFps = kDefaultTargetFps;

    int m_width = 0;
    int m_height = 0;
    int m_fps = 0;
};

#endif // HAL_SENSORS_VIDEOFILE_SOURCE_H
