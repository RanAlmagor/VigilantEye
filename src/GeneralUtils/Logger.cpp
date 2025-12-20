#include "GeneralUtils/Logger.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex); 
    std::cout << "[LOG]: " << message << std::endl;
}