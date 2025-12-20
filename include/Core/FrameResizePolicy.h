#pragma once
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <algorithm>

struct FrameResizePolicy
{
    cv::Size procSize{ 640, 360 };     // (DNN)
    cv::Size displaySize{ 1280, 720 }; // Screen

    
    static inline std::pair<double, double> scaleFactors(const cv::Mat& from, const cv::Mat& to)
    {
        return { (double)to.cols / (double)from.cols, (double)to.rows / (double)from.rows };
    }

    static inline cv::Rect scaleRect(const cv::Rect& r, double sx, double sy)
    {
        return cv::Rect(
            (int)std::round(r.x * sx),
            (int)std::round(r.y * sy),
            (int)std::round(r.width * sx),
            (int)std::round(r.height * sy)
        );
    }

    inline void makeProc(const cv::Mat& full, cv::Mat& proc) const
    {
        cv::resize(full, proc, procSize, 0, 0, cv::INTER_AREA);
    }

    inline void makeDisplay(const cv::Mat& full, cv::Mat& disp) const
    {
        cv::resize(full, disp, displaySize, 0, 0, cv::INTER_AREA);
    }
};
