#include <iostream>
#include <memory>

// החיבורים למערכת שלנו
#include "Core/SystemController.h"
#include "HAL/Sensors/WebcamSource.h"

int main()
{
    try
    {
        // 1. יצירת החיישן (מצלמה מס' 0)
        auto sensor = std::make_unique<WebcamSource>(0);

        // 2. יצירת המנהל והעברת החיישן אליו
        SystemController system(std::move(sensor));

        // 3. הרצה
        system.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}