// ===============================
// SystemController.h
// ===============================
#ifndef CORE_SYSTEM_CONTROLLER_H
#define CORE_SYSTEM_CONTROLLER_H

#include <memory>
#include <atomic>
#include <string>
#include "UI/HUD.h" 
#include "HAL/Sensors/IFrameSource.h"
#include "Core/Pipeline.h"
#include "Vision/ObjectTracker.h"
#include "Vision/DNNObjectDetector.h"

class SystemController
{
public:
    explicit SystemController(std::unique_ptr<IFrameSource> sensor);
    void run();

private:
    std::unique_ptr<IFrameSource> m_sensor;
    Pipeline m_pipeline;
    std::atomic<bool> m_running{ false };

    Vision::ObjectTracker m_tracker;
    DNNObjectDetector* m_detector = nullptr;
    UI::HUD m_hud;

    const std::string BASE_DIR{ "config/models/" };
    const std::string MODEL_PATH{ BASE_DIR + "yolov4-tiny.weights" };
    const std::string CONFIG_PATH{ BASE_DIR + "yolov4-tiny.cfg" };
    const std::string CLASSES_PATH{ BASE_DIR + "coco.names.txt" };
};

#endif // CORE_SYSTEM_CONTROLLER_H