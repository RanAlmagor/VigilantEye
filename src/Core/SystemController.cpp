// ===============================
// SystemController.cpp
// ===============================

#include "Core/SystemController.h"
#include <iostream>
#include <opencv2/highgui.hpp> 

SystemController::SystemController(std::unique_ptr<IFrameSource> sensor) : m_sensor(std::move(sensor))
{

}

void SystemController::run()
{
    std::cout << "[System] Initializing..." << std::endl;

    
    if (!m_sensor || !m_sensor->initialize())
    {
        std::cerr << "[ERROR] Sensor initialization failed!" << std::endl;
        return;
    }

    cv::namedWindow("VigilantEye Output", cv::WINDOW_AUTOSIZE);

    // flag
    m_running = true;

    
    cv::Mat frame;          // original frame
    cv::Mat processedFrame; // after process

    std::cout << "[System] Running. Press ESC to exit." << std::endl;

    
    while (m_running)
    {
        
        if (!m_sensor->capture(frame))
        {
            std::cerr << "[System] Stream ended or capture failed." << std::endl;
            break; 
        }
       
      
        m_pipeline.process(frame, processedFrame);

       
        if (!processedFrame.empty())
        {
            cv::imshow("VigilantEye Output", processedFrame);
        }

        
        char key = (char)cv::waitKey(30); 

        if (key == 27) // ESC
        {
            m_running = false;
        }
    }

    
    std::cout << "[System] Shutdown complete." << std::endl;

   
    if (m_sensor) 
    {
        m_sensor->stop();
    }

    cv::destroyAllWindows();
}