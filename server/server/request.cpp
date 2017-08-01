#include "request.hpp"
#include <unistd.h>
#include "util.hpp"

#define REQUEST_BUFFER_SIZE (128)
#define MAX_BODY_SIZE (2048)

#define REGEX_METHOD "GET|POST"
#define REGEX_REQUEST_URI "\\S+"
#define REGEX_HTTP_VERSION "HTTP\\/1\\.1"
#define REGEX_HEADER_KEY "\\S+"
#define REGEX_HEADER_VALUE ".+"

std::regex Request::regexRequestLine("^(" REGEX_METHOD ") (" REGEX_REQUEST_URI ") (" REGEX_HTTP_VERSION ")$");
std::regex Request::regexRequestHeader("^(" REGEX_HEADER_KEY "): (" REGEX_HEADER_VALUE ")$");
std::regex Request::regexRequestURI("^(\\/.*?)(?:\\?(.*))?$");

Request::Request(int socket) {
    this->socket = socket;
    badRequest = !read();
}

bool Request::read() {
    std::string line;
    std::cmatch cm;
    
    // Read request line
    line = readLine();
    if(!std::regex_search(line.c_str(), cm, regexRequestLine)) return false;
    
    method = cm[1];
    requestURI = cm[2];
    httpVersion = cm[3];
    
    // Decode request URI     // TODO properly decode url
    for(int i = 0;i < requestURI.length(); ++i) if(requestURI[i] == '+') requestURI[i] = ' ';
    
    // Parse URI
    if(!std::regex_search(requestURI.c_str(), cm, regexRequestURI)) return false;
    requestPath = cm[1];
    requestQuery = cm[2];
    
    // May not contain '..'
    if(requestURI.find("..") != std::string::npos) return false;
    
    // Read headers
    while(true) {
        line = readLine();
        if(line.length() == 0) break;
        if(!std::regex_search(line.c_str(), cm, regexRequestHeader)) return false;
        headers.set(cm[1], cm[2]);
    }
    
    // Read body in case of POST request
    if(method.compare("POST") == 0) {
        std::string contentLength = headers.get("Content-Length");
        if(contentLength.length() == 0) return false;
        if(!Util::isInteger(contentLength)) return false;
        int length = stoi(contentLength);
        if(length > MAX_BODY_SIZE) return false;
        
        char buffer[length];
        char* ptr = buffer;
        size_t bytes = 0;
        do {
            bytes += ::read(socket, ptr, length - bytes);
            ptr += bytes;
        }
        while(bytes < length);
        body = std::string(buffer, length);
    }
    else {
        body = "";
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

