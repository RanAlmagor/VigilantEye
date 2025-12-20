#ifndef HAL_SENSORS_VIDEOFILE_SOURCE_H
#define HAL_SENSORS_VIDEOFILE_SOURCE_H

#include "IFrameSource.h"
#include <opencv2/videoio.hpp>
#include <string>

class VideoFileSource : public IFrameSource
{
public:
    static constexpr int kDefaultTargetFps = 30;

    explicit VideoFileSource(std::string filePath,
        int targetFps = kDefaultTargetFps,
        bool loop = true);

    // Rule of Five
    VideoFileSource(const VideoFileSource&) = delete;
    VideoFileSource& operator=(const VideoFileSource&) = delete;
    VideoFileSource(VideoFileSource&&) noexcept = default;
    VideoFileSource& operator=(VideoFileSource&&) noexcept = default;

    ~VideoFileSource() override { stop(); }

    // ===============================
    // IFrameSource Implementation
    // ===============================
    bool initialize() override;
    bool capture(cv::Mat& frame) override;
    void stop() noexcept override;

    const std::string& getSourceName() const override { return m_sourceName; }

    int getWidth() const override { return m_width; }
    int getHeight() const override { return m_height; }

    // ?? m_fps ?? ???? ??? ??????, ????? targetFps
    double getFPS() const override { return (m_fps > 1.0 ? m_fps : (double)m_targetFps); }

    const std::string& path() const noexcept { return m_filePath; }

    // optional
    bool isLooping() const noexcept { return m_loop; }
    void setLoop(bool loop) noexcept { m_loop = loop; }

private:
    bool rewindToStart();

private:
    cv::VideoCapture m_cap;

    std::string m_filePath;
    std::string m_sourceName;

    int m_targetFps = kDefaultTargetFps;

    int m_width = 0;
    int m_height = 0;
    double m_fps = (double)kDefaultTargetFps;

    bool m_loop = true;
};

#endif // HAL_SENSORS_VIDEOFILE_SOURCE_H
