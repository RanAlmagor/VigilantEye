#ifndef UI_HUD_H
#define UI_HUD_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string> 
#include "Vision/DNNObjectDetector.h" 

namespace UI {

    class HUD {
    public:
        HUD();
        ~HUD() = default;

        /**
         * @brief Draws the tactical overlay on the given frame.
         * @param frame The current video frame (input/output).
         * @param detections List of detected objects to display.
         * @param fps Current Frames Per Second value.
         */
        void update(cv::Mat& frame, const std::vector<Detection>& detections, double fps);

    private:
        void drawBracket(cv::Mat& frame, const cv::Rect& rect, const cv::Scalar& color);
        void drawCrosshair(cv::Mat& frame, const cv::Point& center, const cv::Scalar& color);
        void drawStatusBar(cv::Mat& frame, int objectCount, double fps);

        const cv::Scalar m_colorNormal;
        const cv::Scalar m_colorAlert;
        const cv::Scalar m_colorText;
    };
}

#endif // UI_HUD_H