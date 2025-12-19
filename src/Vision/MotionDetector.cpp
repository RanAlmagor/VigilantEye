#include "Vision/MotionDetector.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

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

    // 1. חישוב ההפרש
    cv::absdiff(m_prevFrame, gray, delta);
    cv::threshold(delta, mask, 25, 255, cv::THRESH_BINARY);

    // === התיקון הגדול: איחוד שברים (Morphology) ===

    // יצירת "מכחול" (Kernel) מרובע בגודל 15 על 15 פיקסלים
    // זה הרבה יותר חזק מהברירת מחדל שהיתה קודם
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

    // פעולת Dilate: מרחיבה את הלבן ומחברת כתמים קרובים
    cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 2);

    // עדכון הזיכרון
    m_prevFrame = gray.clone();

    // === ציור ===
    output = input.clone();
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours)
    {
        // סינון רעשים: התעלמות מגושים קטנים
        // הגדלנו את הסף ל-1000 כדי לתפוס רק גופים משמעותיים (בני אדם)
        if (cv::contourArea(contour) < 1000)
            continue;

        cv::Rect box = cv::boundingRect(contour);

        // ציור ריבוע ירוק
        cv::rectangle(output, box, cv::Scalar(0, 255, 0), 2);

        // בונוס: כתיבת טקסט "TARGET" מעל הריבוע
        cv::putText(output, "TARGET", box.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }
}