#include <iostream>
#include <memory>
#include "Core/SystemController.h"
#include "HAL/Sensors/VideoFileSource.h" // <--- חזרנו לזה
// #include "HAL/Sensors/WebcamSource.h" // <--- את זה נשים בהערה

int main()
{
    try {
        std::cout << "--- VigilantEye System Startup (Simulation Mode) ---" << std::endl;

     
        auto source = std::make_unique<VideoFileSource>("demo.mp4");

        if (!source->initialize()) {
            std::cerr << "[Fatal] Video file not found or failed to open." << std::endl;
            std::cout << "Make sure 'demo.mp4' is in the executable folder." << std::endl;
            std::cin.get();
            return -1;
        }

        SystemController system(std::move(source));
        system.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::cin.get();
        return -1;
    }

    return 0;
}