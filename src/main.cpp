#include <memory>
#include "Core/SystemController.h"
#include "HAL/Sensors/VideoFileSource.h"
#include "GeneralUtils/Logger.h"

int main()
{
    try {
        Logger::getInstance().log("--- VigilantEye System Startup (Simulation Mode) ---");

        auto source = std::make_unique<VideoFileSource>("demo.mp4");

        SystemController system(std::move(source));
        system.run();
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Fatal Error: ") + e.what());
        return -1;
    }

    return 0;
}
