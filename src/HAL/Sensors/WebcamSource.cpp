#include "HAL/Sensors/WebcamSource.h"
#include <iostream>
#include <string>

// Constructor
WebcamSource::WebcamSource(int cameraIndex, int targetWidth, int targetHeight, int targetFps)
    : m_deviceId(cameraIndex),
    m_targetWidth(targetWidth),
    m_targetHeight(targetHeight),
    m_targetFps(targetFps),
    m_sourceName("Webcam_" + std::to_string(cameraIndex))
{
    // Lazy Initialization: We don't open the camera here yet.
}

bool WebcamSource::initialize() 
{
    stop();
    std::cout << "[HAL] Opening " << m_sourceName << "..." << std::endl;

    // 1. Open Hardware
    m_cap.open(m_deviceId, cv::CAP_ANY);

    if (!m_cap.isOpened()) 
    {
        std::cerr << "[ERROR] Failed to open camera index: " << m_deviceId << std::endl;
        return false;
    }

    // 2. Configure (Request)
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, m_targetWidth);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, m_targetHeight);
    m_cap.set(cv::CAP_PROP_FPS, m_targetFps);

    // 3. Verify (Actual)
    m_actualWidth = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    m_actualHeight = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    m_actualFps = static_cast<int>(m_cap.get(cv::CAP_PROP_FPS));

    std::cout << "[HAL] Initialized " << m_sourceName
        << " | Actual: " << m_actualWidth << "x" << m_actualHeight
        << " | FPS: " << m_actualFps
        << std::endl;


    return true;
}

bool WebcamSource::capture(cv::Mat& frame) {
    if (!m_cap.isOpened()) return false;

    m_cap >> frame;

    if (frame.empty()) {
        return false;
    }
    return true;
}


