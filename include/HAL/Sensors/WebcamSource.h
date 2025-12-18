#ifndef HAL_SENSORS_WEBCAM_SOURCE_H
#define HAL_SENSORS_WEBCAM_SOURCE_H

#include "IFrameSource.h"
#include <opencv2/videoio.hpp> 
#include <string>


class WebcamSource : public IFrameSource
{
public:
 
    static constexpr int kDefaultDeviceId = 0;
    static constexpr int kDefaultWidth = 640;
    static constexpr int kDefaultHeight = 480;
    static constexpr int kDefaultFps = 30;

    // Constructor
    explicit WebcamSource(int cameraIndex = kDefaultDeviceId,
        int targetWidth = kDefaultWidth,
        int targetHeight = kDefaultHeight,
        int targetFps = kDefaultFps);

    // Rule of Five
    WebcamSource(const WebcamSource&) = delete;
    WebcamSource(WebcamSource&&) noexcept = default;
    WebcamSource& operator=(const WebcamSource&) = delete;
    WebcamSource& operator=(WebcamSource&&) noexcept = default;

    // Interface
    bool initialize() override;
    bool capture(cv::Mat& frame) override;
    const std::string& getSourceName() const override { return m_sourceName; }
    void stop() noexcept;
    ~WebcamSource() override { stop(); }


    // Getters
    int width()  const { return m_actualWidth; }
    int height() const { return m_actualHeight; }
    int fps()    const { return m_actualFps; }

private:
    cv::VideoCapture m_cap;
    std::string m_sourceName;

    
    int m_deviceId;
    int m_targetWidth;
    int m_targetHeight;
    int m_targetFps;

    
    int m_actualWidth = 0;
    int m_actualHeight = 0;
    int m_actualFps = 0;
};

#endif // HAL_SENSORS_WEBCAM_SOURCE_H