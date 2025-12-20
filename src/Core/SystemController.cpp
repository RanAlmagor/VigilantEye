#include "Core/SystemController.h"
#include "GeneralUtils/Logger.h" 
#include <opencv2/highgui.hpp>
#include <string>
#include <opencv2/core/utility.hpp> 

const std::string WINDOW_NAME = "VigilantEye - Tactical Feed";

SystemController::SystemController(std::unique_ptr<IFrameSource> sensor)
    : m_sensor(std::move(sensor))
{
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);

    auto dnnDetector = std::make_unique<DNNObjectDetector>();
    Logger::getInstance().log("[System] Initializing AI...");

  
    if (dnnDetector->init(MODEL_PATH, CONFIG_PATH, CLASSES_PATH)) {
        Logger::getInstance().log("[System] AI Model loaded successfully.");
        m_detector = dnnDetector.get(); 
        m_pipeline.addAlgorithm(std::move(dnnDetector)); 
    }
    else {
        Logger::getInstance().log("[System] [WARNING] AI failed to load. Running in Camera-Only mode.");
        m_detector = nullptr;
    }
}

void SystemController::run()
{
    m_running = true;
    Logger::getInstance().log("[System] Running. Press ESC to stop.");


    if (!m_sensor->initialize()) {
        Logger::getInstance().log("[System] [ERROR] Sensor initialization failed!");
        return;
    }

    cv::Mat frame, output;
    cv::TickMeter fpsTimer; 
    double currentFPS = 0.0;
    bool isFullscreenSet = false;

    while (m_running)
    {
        fpsTimer.start(); 

        // 1. Capture
        if (!m_sensor->capture(frame)) {
            Logger::getInstance().log("[System] Stream ended or capture failed.");
            break;
        }

        if (frame.empty()) {
            fpsTimer.stop();
            continue;
        }

   
        m_pipeline.process(frame, output);
        cv::Mat& displayFrame = output.empty() ? frame : output;

       
        if (m_detector != nullptr)
        {
            
            const auto& detections = m_detector->getLatestDetections();

      
            std::vector<cv::Rect> trackerInput;
            for (const auto& det : detections) trackerInput.push_back(det.box);
            m_tracker.update(trackerInput);

            
            m_hud.update(displayFrame, detections, currentFPS);
        }

   
        cv::imshow(WINDOW_NAME, displayFrame);

        
        if (!isFullscreenSet) {
            cv::setWindowProperty(WINDOW_NAME, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
            isFullscreenSet = true;
        }

 
        if (cv::waitKey(1) == 27) m_running = false;

        fpsTimer.stop();
        currentFPS = fpsTimer.getFPS(); 
    
        fpsTimer.reset();
    }

    m_sensor->stop();
    cv::destroyAllWindows();
    Logger::getInstance().log("[System] System shutdown complete.");
}