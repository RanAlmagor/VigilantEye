#ifndef HAL_SENSORS_WEBCAM_SOURCE_H
#define HAL_SENSORS_WEBCAM_SOURCE_H

#include "IFrameSource.h"
#include <opencv2/videoio.hpp> // VideoCapture

/**
 * @brief Concrete implementation for USB Webcams.
 */
class WebcamSource : public IFrameSource
{
public:
    explicit WebcamSource(int cameraIndex = 0, int targetWidth = 640, int targetHeight = 480, int targetFps = 30);
    WebcamSource(const WebcamSource&) = delete;
    WebcamSource(WebcamSource&&) noexcept = default;
    WebcamSource& operator=(const WebcamSource&) = delete;
    WebcamSource& operator=(WebcamSource&&) noexcept = default;
    bool initialize() override;
    bool capture(cv::Mat& frame) override;
    const std::string& getSourceName() const override;
    ~WebcamSource() override = default;

private:
    // --- Data Members ---
    cv::VideoCapture m_cap;
    std::string m_sourceName;

    int m_deviceId = 0;
    int m_targetWidth = 640;
    int m_targetHeight = 480;
    int m_targetFps = 30;
    int m_actualWidth = 0, m_actualHeight = 0, m_actualFps = 0;

}; 













#endif // HAL_SENSORS_WEBCAM_SOURCE_H