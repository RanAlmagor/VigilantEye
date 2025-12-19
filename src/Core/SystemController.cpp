#include "Core/SystemController.h"
#include "Vision/DNNObjectDetector.h"
#include <iostream>
#include <opencv2/highgui.hpp> 

SystemController::SystemController(std::unique_ptr<IFrameSource> sensor)
    : m_sensor(std::move(sensor))
{
    
    auto dnnDetector = std::make_unique<DNNObjectDetector>();

    std::cout << "[System] Initializing AI..." << std::endl;
    if (!dnnDetector->init(MODEL_PATH, CONFIG_PATH, CLASSES_PATH))
    {
        std::cerr << "[System] Warning: AI Init failed. Running in generic mode." << std::endl;
    }
    else
    {
        std::cout << "[System] AI Model loaded." << std::endl;
    }

    
    m_pipeline.addAlgorithm(std::move(dnnDetector));
}

void SystemController::run()
{
    m_running = true;
    std::cout << "[System] Running. Press ESC to stop." << std::endl;

    cv::Mat frame, output;

    while (m_running)
    {
        if (!m_sensor->capture(frame)) break;
        if (frame.empty()) continue;

       
        m_pipeline.process(frame, output);

       
        if (!output.empty()) {
            cv::imshow("VigilantEye", output);
        }
        else {
            cv::imshow("VigilantEye", frame);
        }

        if (cv::waitKey(1) == 27) m_running = false;
    }
    cv::destroyAllWindows();
}