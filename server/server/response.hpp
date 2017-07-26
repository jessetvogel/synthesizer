#ifndef response_hpp
#define response_hpp

// https://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html

#include <string>

#include "headers.hpp"

class Response {

    int socket;
    
    std::string status;
    Headers headers;
    
public:
    
    Response(int);
    
    inline void setStatus(std::string status) { this->status = status; }
    inline void setHeader(std::string field, std::string value) { headers.set(field, value); }
    
    bool writeStatus();
    bool writeHeaders();
    
    bool write(std::string);
    bool writeLine();
    bool writeLine(std::string);
    
    bool writeFile(std::string);

};

#endif
