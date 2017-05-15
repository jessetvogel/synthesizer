#include "log.hpp"

#include <iostream>

void Log::output(const char* message) {
    std::cout << message << std::endl;
}

void Log::warning(const char* message) {
    std::cout << "[WARNING] " << message << std::endl;
}

void Log::error(const char* message) {
    std::cout << "[ ERROR ] " << message << std::endl;
}
