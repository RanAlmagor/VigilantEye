// HUD.cpp
#include "UI/HUD.h"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <string>

namespace UI {

    HUD::HUD()
        : m_colorNormal(255, 200, 0)
        , m_colorAlert(0, 0, 255)
        , m_colorText(255, 255, 255)
    {
    }

    static inline double uiScaleFor(const cv::Mat& frame)
    {
      
        return std::max(0.65, (double)frame.cols / 1280.0);
    }

    static inline void drawTextWithBg(
        cv::Mat& img,
        const std::string& text,
        cv::Point org,
        double fontScale,
        int thickness,
        const cv::Scalar& textColor,
        const cv::Scalar& bgColor
    ) {
        int baseLine = 0;
        cv::Size ts = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseLine);

        int pad = std::max(3, (int)std::round(6 * fontScale));
        cv::Rect bg(org.x - pad, org.y - ts.height - pad, ts.width + 2 * pad, ts.height + baseLine + 2 * pad);

        bg.x = std::max(0, bg.x);
        bg.y = std::max(0, bg.y);
        bg.width = std::min(bg.width, img.cols - bg.x);
        bg.height = std::min(bg.height, img.rows - bg.y);

        cv::rectangle(img, bg, bgColor, cv::FILLED);
        cv::putText(img, text, org, cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness, cv::LINE_AA);
    }

    void HUD::update(cv::Mat& frame, const std::vector<Detection>& detections, double fps)
    {
        const double s = uiScaleFor(frame);

        drawStatusBar(frame, (int)detections.size(), fps);

        for (const auto& det : detections)
        {
            const cv::Scalar color = (det.confidence > 0.75) ? m_colorAlert : m_colorNormal;

            const int thickness = std::max(1, (int)std::round(2 * s));

            drawBracket(frame, det.box, color);

            cv::Point center(det.box.x + det.box.width / 2, det.box.y + det.box.height / 2);
            drawCrosshair(frame, center, color);

       
            const int confPct = (int)std::round(det.confidence * 100.0);
            std::string label = det.label + " " + std::to_string(confPct) + "%";

            double fontScale = 0.55 * s;

            cv::Point org(det.box.x, det.box.y - (int)std::round(8 * s));
            if (org.y < 20) org.y = det.box.y + (int)std::round(22 * s);

            drawTextWithBg(frame, label, org, fontScale, thickness, m_colorText, cv::Scalar(0, 0, 0));
        }
    }

    void HUD::drawBracket(cv::Mat& frame, const cv::Rect& rect, const cv::Scalar& color)
    {
        const double s = uiScaleFor(frame);
        int t = std::max(1, (int)std::round(2 * s));

        int w = std::max(10, rect.width / 4);
        int h = std::max(10, rect.height / 4);

        int x = rect.x;
        int y = rect.y;
        int xw = rect.x + rect.width;
        int yh = rect.y + rect.height;

        cv::line(frame, { x, y }, { x + w, y }, color, t, cv::LINE_AA);
        cv::line(frame, { x, y }, { x, y + h }, color, t, cv::LINE_AA);

        cv::line(frame, { xw, y }, { xw - w, y }, color, t, cv::LINE_AA);
        cv::line(frame, { xw, y }, { xw, y + h }, color, t, cv::LINE_AA);

        cv::line(frame, { x, yh }, { x + w, yh }, color, t, cv::LINE_AA);
        cv::line(frame, { x, yh }, { x, yh - h }, color, t, cv::LINE_AA);

        cv::line(frame, { xw, yh }, { xw - w, yh }, color, t, cv::LINE_AA);
        cv::line(frame, { xw, yh }, { xw, yh - h }, color, t, cv::LINE_AA);
    }

    void HUD::drawCrosshair(cv::Mat& frame, const cv::Point& center, const cv::Scalar& color)
    {
        const double s = uiScaleFor(frame);
        int size = (int)std::round(12 * s);
        int gap = (int)std::round(4 * s);

        cv::line(frame, { center.x - size, center.y }, { center.x - gap, center.y }, color, 1, cv::LINE_AA);
        cv::line(frame, { center.x + gap, center.y }, { center.x + size, center.y }, color, 1, cv::LINE_AA);

        cv::line(frame, { center.x, center.y - size }, { center.x, center.y - gap }, color, 1, cv::LINE_AA);
        cv::line(frame, { center.x, center.y + gap }, { center.x, center.y + size }, color, 1, cv::LINE_AA);

        cv::circle(frame, center, std::max(1, (int)std::round(2 * s)), color, cv::FILLED, cv::LINE_AA);
    }

    void HUD::drawStatusBar(cv::Mat& frame, int objectCount, double fps)
    {
        const double s = uiScaleFor(frame);
        int barH = (int)std::round(34 * s);

        cv::Rect r(0, std::max(0, frame.rows - barH), frame.cols, std::min(barH, frame.rows));
        cv::Mat roi = frame(r);

  
        cv::Mat black(roi.size(), roi.type(), cv::Scalar(0, 0, 0));
        cv::addWeighted(black, 0.65, roi, 0.35, 0, roi);

        int fpsInt = (int)std::round(fps);
        std::string text = "ARMED | TGT: " + std::to_string(objectCount) + " | FPS: " + std::to_string(fpsInt);

        double fontScale = 0.8 * s;
        int thickness = std::max(1, (int)std::round(2 * s));

        cv::putText(frame, text,
            { (int)std::round(12 * s), frame.rows - (int)std::round(10 * s) },
            cv::FONT_HERSHEY_SIMPLEX,
            fontScale,
            m_colorNormal,
            thickness,
            cv::LINE_AA);
    }

} 
