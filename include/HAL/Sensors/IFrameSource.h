// ===============================
// IFrameSource.h
// ===============================

#ifndef HAL_SENSORS_IFRAME_SOURCE_H
#define HAL_SENSORS_IFRAME_SOURCE_H

#include <opencv2/core/mat.hpp>
#include <string>

/**
 * @brief Interface for any frame provider in the system.
 * This contract ensures our Tracking Engine is decoupled from the specific hardware
 * (USB Webcam, Video File, RTSP Stream, etc.).
 */
class IFrameSource
{
public:
    virtual ~IFrameSource() = default;

    /**
     * @brief Initialize the hardware connection or open the file.
     * @return true if successful, false otherwise.
     */
    virtual bool initialize() = 0;

    /**
     * @brief Capture the next frame from the source.
     * @param frame [Out] The matrix to store the image data.
     * @return true if a frame was captured, false if stream ended or error occurred.
     */
    virtual bool capture(cv::Mat& frame) = 0;

    /**
     * @brief Release resources and close connection.
     */
    virtual void stop() noexcept = 0;

    // ==========================================
    // Getters for Source Properties
    // ==========================================

    /**
     * @brief Get the source width in pixels.
     */
    virtual int getWidth() const = 0;

    /**
     * @brief Get the source height in pixels.
     */
    virtual int getHeight() const = 0;

    /**
     * @brief Get the Frames Per Second (FPS) of the source.
     * Important for time-dependent tracking calculations.
     */
    virtual double getFPS() const = 0;

    /**
     * @brief Get identifier name (e.g., "Webcam_0" or "VideoFile_Trial1").
     */
    virtual const std::string& getSourceName() const = 0;
};

#endif // HAL_SENSORS_IFRAME_SOURCE_H