#include "Vision/Utils/Visualizer.h"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

namespace Vision {
    namespace Utils {

        vector<string> Visualizer::loadClasses(const string& path) {
            vector<string> classes;
            ifstream ifs(path.c_str());
            string line;
            while (getline(ifs, line)) {
                if (!line.empty()) classes.push_back(line);
            }
            return classes;
        }

        void Visualizer::drawPrediction(Mat& frame, const string& className, float conf, const Rect& box) {
            
            rectangle(frame, box, Scalar(0, 255, 0), 2);

            
            string label = format("%.2f", conf);
            if (!className.empty()) {
                label = className + ": " + label;
            }

            int baseLine;
            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            int top = max(box.y, labelSize.height);

            rectangle(frame, Point(box.x, top - round(1.5 * labelSize.height)),
                Point(box.x + round(1.5 * labelSize.width), top + baseLine),
                Scalar(255, 255, 255), FILLED);

            putText(frame, label, Point(box.x, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);
        }
    }
}