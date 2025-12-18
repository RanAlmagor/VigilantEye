#ifndef HAL_SENSORS_IFRAME_SOURCE_H
#define HAL_SENSORS_IFRAME_SOURCE_H
#include <opencv2/core/mat.hpp>
#include <string>
/**
 * @brief Interface for any frame provider in the system.
 * This ensures our Tracking Engine doesn't care if the image comes from
 * a USB Webcam, a Video File, or an RTSP Stream.
 */

class IFrameSource
{
public:


	/**
	 * @brief Initialize the hardware connection.
	 * @return true if successful, false otherwise.
	 */

	virtual bool initialize() = 0;


	/**
	 * @brief Capture the next frame.
	 * @param frame [Out] The matrix to store the image data.
	 * @return true if a frame was captured, false if stream ended/error.
	 */

	virtual bool capture(cv::Mat& frame) = 0;

	/**
	 * @brief Get identifier name (e.g., "Webcam_0" or "VideoFile").
	 */
	virtual const std::string& getSourceName() const = 0;


	virtual ~IFrameSource() = default;

};

#endif // HAL_SENSORS_IFRAME_SOURCE_H

