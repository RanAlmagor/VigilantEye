#include "UI/HUD.h"
#include <string>
#include <algorithm> 

namespace UI {

    HUD::HUD()
        : m_colorNormal(255, 200, 0)   
        , m_colorAlert(0, 0, 255)      
        , m_colorText(255, 255, 255)  
    {
    }

    void HUD::update(cv::Mat& frame, const std::vector<Detection>& detections, double fps)
    {
        
        drawStatusBar(frame, detections.size(), fps);

      
        for (const auto& det : detections) {
            
            cv::Scalar color = (det.confidence > 0.75) ? m_colorAlert : m_colorNormal;

            
            drawBracket(frame, det.box, color);

            
            cv::Point center(det.box.x + det.box.width / 2, det.box.y + det.box.height / 2);
            drawCrosshair(frame, center, color);

        
            std::string label = det.label + " " + std::to_string((int)(det.confidence * 100)) + "%";

            
            int baseLine;
            cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            int top = std::max(det.box.y, labelSize.height);

          
            cv::rectangle(frame,
                cv::Point(det.box.x, top - labelSize.height - 5),
                cv::Point(det.box.x + labelSize.width, top + baseLine),
                cv::Scalar(0, 0, 0), cv::FILLED);

            // ???? ????? ????
            cv::putText(frame, label, cv::Point(det.box.x, top - 2),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, m_colorText, 1);
        }
    }

    void HUD::drawBracket(cv::Mat& frame, const cv::Rect& rect, const cv::Scalar& color)
    {
        
        int w = rect.width / 5;
        int h = rect.height / 5;
        int t = 2; 

        int x = rect.x;
        int y = rect.y;
        int xw = rect.x + rect.width;
        int yh = rect.y + rect.height;

      
        cv::line(frame, { x, y }, { x + w, y }, color, t);
        cv::line(frame, { x, y }, { x, y + h }, color, t);

     
        cv::line(frame, { xw, y }, { xw - w, y }, color, t);
        cv::line(frame, { xw, y }, { xw, y + h }, color, t);

   
        cv::line(frame, { x, yh }, { x + w, yh }, color, t);
        cv::line(frame, { x, yh }, { x, yh - h }, color, t);

     
        cv::line(frame, { xw, yh }, { xw - w, yh }, color, t);
        cv::line(frame, { xw, yh }, { xw, yh - h }, color, t);
    }

    void HUD::drawCrosshair(cv::Mat& frame, const cv::Point& center, const cv::Scalar& color)
    {
        int size = 10;
        int gap = 4; 

       
        cv::line(frame, { center.x - size, center.y }, { center.x - gap, center.y }, color, 1);
        cv::line(frame, { center.x + gap, center.y }, { center.x + size, center.y }, color, 1);

       
        cv::line(frame, { center.x, center.y - size }, { center.x, center.y - gap }, color, 1);
        cv::line(frame, { center.x, center.y + gap }, { center.x, center.y + size }, color, 1);
    }

    void HUD::drawStatusBar(cv::Mat& frame, int objectCount, double fps)
    {
        int barH = 35; 

        
        cv::Mat overlay;
        frame.copyTo(overlay);

       
        cv::rectangle(overlay, cv::Rect(0, frame.rows - barH, frame.cols, barH), cv::Scalar(0, 0, 0), -1);

      
        cv::addWeighted(overlay, 0.7, frame, 0.3, 0, frame);

        
        std::string fpsStr = std::to_string((int)fps);
        std::string text = "SYSTEM: ARMED  |  TARGETS: " + std::to_string(objectCount) + "  |  FPS: " + fpsStr;

    
        cv::putText(frame, text, { 20, frame.rows - 10 }, cv::FONT_HERSHEY_PLAIN, 1.2, m_colorNormal, 1, cv::LINE_AA);
    }

} 