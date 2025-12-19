#include <iostream>
#include <memory>
#include "Core/SystemController.h"
#include "HAL/Sensors/WebcamSource.h"

int main()
{
    try {
        std::cout << "--- VigilantEye Startup ---" << std::endl;

  
        auto camera = std::make_unique<WebcamSource>(0);

        
        if (!camera->initialize()) {
            std::cerr << "[Fatal] Could not initialize camera!" << std::endl;
            std::cin.get(); 
            return -1;
        }

        
        SystemController system(std::move(camera));

       
        system.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::cin.get();
        return -1;
    }

    return 0;
}