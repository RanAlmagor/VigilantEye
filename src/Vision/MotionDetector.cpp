#include "Vision/MotionDetector.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector> 

void MotionDetector::detect(const cv::Mat& input, cv::Mat& output)
{
    if (input.empty()) return;


    cv::Mat gray, mask, delta;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);

   
    if (m_prevFrame.empty())
    {
        m_prevFrame = gray.clone();
        output = input.clone(); 
        return;
    }

 
    cv::absdiff(m_prevFrame, gray, delta);
    cv::threshold(delta, mask, 25, 255, cv::THRESH_BINARY);


    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
    cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 2);


    m_prevFrame = gray.clone();


    output = input.clone();


    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours)
    {
       
        if (cv::contourArea(contour) < 1000)
            continue;

        cv::Rect box = cv::boundingRect(contour);

   
        cv::rectangle(output, box, cv::Scalar(0, 255, 255), 2); // צהוב
        cv::putText(output, "MOTION", box.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
    }
}