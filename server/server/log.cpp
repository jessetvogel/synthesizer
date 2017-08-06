#include "log.hpp"
#include <fstream>

void Log::write(std::string message) {
    std::fstream file;
    file.open("log/log.txt", std::fstream::app);
    file << message;
    file.close();
}


void Log::writeLine(std::string message) {
    write(message + "\n");
}
