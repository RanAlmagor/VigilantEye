#include <iostream>
#include <opencv2/highgui.hpp> // בשביל imshow, waitKey
#include "HAL/Sensors/WebcamSource.h"

int main() {
    // יצירת אובייקט המצלמה
    WebcamSource cam;

    std::cout << "[MAIN] System Starting..." << std::endl;

    // אתחול
    if (!cam.initialize()) {
        std::cerr << "[ERROR] Could not initialize camera!" << std::endl;
        return -1;
    }

    std::cout << "[MAIN] Camera is ready. Press 'ESC' to exit." << std::endl;

    cv::Mat frame;

    // לולאה אינסופית
    while (true) {

        // לכידת תמונה
        if (!cam.capture(frame)) {
            std::cerr << "[ERROR] Frame capture failed!" << std::endl;
            break;
        }

        // הצגה על המסך
        cv::imshow("VigilantEye Camera Test", frame);

        // המתנה של 10 מילישניות ללחיצת מקש
        char key = (char)cv::waitKey(10);
        if (key == 27) { // 27 = ESC
            break;
        }
    }

    return 0;
}