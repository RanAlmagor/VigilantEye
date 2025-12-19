#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

namespace Vision {
    namespace Utils {

        class Visualizer {
        public:
         
            static std::vector<std::string> loadClasses(const std::string& path);

            static void drawPrediction(cv::Mat& frame, const std::string& label, float conf, const cv::Rect& box);
        };

    }
}