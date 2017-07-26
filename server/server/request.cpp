#include "request.hpp"
#include <unistd.h>
#include <iostream>

#define REQUEST_BUFFER_SIZE (128)

#define REGEX_METHOD "GET|POST"
#define REGEX_REQUEST_URI "\\S+"
#define REGEX_HTTP_VERSION "HTTP\\/1\\.1"
#define REGEX_HEADER_KEY "\\S+"
#define REGEX_HEADER_VALUE ".+"

std::regex Request::requestLine("^(" REGEX_METHOD ") (" REGEX_REQUEST_URI ") (" REGEX_HTTP_VERSION ")$");
std::regex Request::requestHeader("^(" REGEX_HEADER_KEY "): (" REGEX_HEADER_VALUE ")$");

Request::Request(int socket) {
    this->socket = socket;
    badRequest = !read();
}

bool Request::read() {
    std::string line;
    std::cmatch cm;
    
    // Read request line
    line = readLine();
    if(!std::regex_search(line.c_str(), cm, requestLine)) return false;
    
    method = cm[1];
    requestURI = cm[2];
    httpVersion = cm[3];
    
    // Decode request URI     // TODO properly decode url
    for(int i = 0;i < requestURI.length(); ++i) if(requestURI[i] == '+') requestURI[i] = ' ';
    
    // May not contain '..'
    if(requestURI.find("..") != std::string::npos) return false;
    
    // Read headers
    while(true) {
        line = readLine();
        if(line.length() == 0) break;
        if(!std::regex_search(line.c_str(), cm, requestHeader)) return false;
        headers.set(cm[1], cm[2]);
    }
    
    return true;
}

std::string Request::readLine() {
    char buffer[REQUEST_BUFFER_SIZE];
    char* ptr = buffer;
    size_t bytes;
    do bytes = ::read(socket, ptr, 1);
    while(bytes > 0 && *(ptr++) != '\n');
    long length = ptr - buffer - 2; // TODO something with CRLF's
    if(length < 0) length = 0;
    return std::string(buffer, length);
}
