#include "Core/SystemController.h"
#include <iostream>
#include <opencv2/highgui.hpp> 

SystemController::SystemController(std::unique_ptr<IFrameSource> sensor)
    : m_sensor(std::move(sensor))
{
  
    auto dnnDetector = std::make_unique<DNNObjectDetector>();

    std::cout << "[System] Initializing AI..." << std::endl;

    bool initSuccess = dnnDetector->init(MODEL_PATH, CONFIG_PATH, CLASSES_PATH);

    if (initSuccess)
    {
        std::cout << "[System] AI Model loaded successfully." << std::endl;

       
        m_detector = dnnDetector.get();

        
        m_pipeline.addAlgorithm(std::move(dnnDetector));
    }
    else
    {
        std::cerr << "[System] Warning: AI failed to load. Running in Camera-Only mode." << std::endl;
        m_detector = nullptr; 
    }
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

   
        if (m_detector != nullptr)
        {
           
            const auto& detections = m_detector->getLatestDetections();

            
            m_tracker.update(detections);

          
            if (!output.empty()) {
                m_tracker.draw(output);
            }
        }

      
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