#ifndef request_hpp
#define request_hpp

// https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html

#include <string>
#include <regex>

#include "headers.hpp"

class Request {

    static std::regex regexRequestLine;
    static std::regex regexRequestHeader;
    
    static std::regex regexRequestURI;
    
    int socket;
    
    std::string method;
    std::string requestURI;
    std::string requestPath;
    std::string requestQuery;
    std::string httpVersion;
    Headers headers;
    std::string body;
    
    bool badRequest;
    
public:
    
    Request(int);
    
    bool read();
    
    std::string readLine();
    
    inline bool bad() { return badRequest; }
    
    inline std::string getMethod() { return method; }
    inline std::string getRequestURI() { return requestURI; }
    inline std::string getRequestPath() { return requestPath; }
    inline std::string getRequestQuery() { return requestQuery; }
    inline std::string getHTTPVersion() { return httpVersion; }
    inline std::string getHeader(std::string field) { return headers.get(field); }
    inline std::string getBody() { return body; }
    
};

#endif
