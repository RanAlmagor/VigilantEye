#include <memory>
#include "Core/SystemController.h"
#include "HAL/Sensors/WebcamSource.h"
#include "GeneralUtils/Logger.h"

int main()
{
    try {
        Logger::getInstance().log("--- VigilantEye System Startup (Webcam Mode) ---");

        // cameraIndex = 0 is usually the default built-in / primary camera
        auto source = std::make_unique<WebcamSource>(0);

        SystemController system(std::move(source));
        system.run();
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Fatal Error: ") + e.what());
        return -1;
    }

    return 0;
}
