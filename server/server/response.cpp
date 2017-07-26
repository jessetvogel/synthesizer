#include "response.hpp"
#include <unistd.h>
#include <fstream>

#define FILE_BUFFER_SIZE (512)

Response::Response(int socket) {
    this->socket = socket;
    
    // Set default status and headers
    status = "200 OK";
    setHeader("Content-Type", "text/plain");
    char date[64]; time_t now = time(0); strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now));
    setHeader("Date", date);
}

bool Response::writeStatus() {
    return write("HTTP/1.1 ") && writeLine(status);
}

bool Response::writeHeaders() {
    return headers.write(this);
}

bool Response::write(std::string string) {
    size_t length = string.length();
    return ::write(socket, string.c_str(), length) == length;
}

bool Response::writeLine() {
    return write("\n");
}

bool Response::writeLine(std::string string) {
    return write(string) && writeLine();
}

bool Response::writeFile(std::string path) {
    std::ifstream file;
    file.open(path);
    if(file.fail()) return false;
    
    char buffer[FILE_BUFFER_SIZE];
    std::streamsize bytes;
    do {
        file.read(buffer, FILE_BUFFER_SIZE);
        bytes = file.gcount();
        ::write(socket, buffer, bytes);
    } while(bytes > 0);
    
    file.close();
    return true;
}
