#include "HAL/Sensors/WebcamSource.h"
#include <iostream>

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
    std::cout << "[HAL] Opening " << m_sourceName << "..." << std::endl;
    // 1. Open Hardware
    m_cap.

}