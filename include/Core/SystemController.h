// ===============================
// SystemController.h
// ===============================

#ifndef CORE_SYSTEM_CONTROLLER_H
#define CORE_SYSTEM_CONTROLLER_H

#include <memory>  
#include <atomic>  
#include "HAL/Sensors/IFrameSource.h"
#include "Core/Pipeline.h"

class SystemController
{
public:

    /**
     * @brief Constructor
     * @param sensor The input source (Webcam or File), ownership is transferred here.
     */
    explicit SystemController(std::unique_ptr<IFrameSource> sensor);

    /**
     * @brief Main loop of the application.
     * Runs until ESC is pressed or stream ends.
     */
    void run();

private:
  
    std::unique_ptr<IFrameSource> m_sensor;
    Pipeline m_pipeline;

    // Control flag
    std::atomic<bool> m_running{ false };
};

#endif // CORE_SYSTEM_CONTROLLER_H