#pragma once
#include <iostream>
#include <string>
#include <mutex>

// Singleton Logger
class Logger {
public:
    
    static Logger& getInstance();

   
    void log(const std::string& message);

    
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

private:
    Logger() {} 
    std::mutex m_mutex; 
};