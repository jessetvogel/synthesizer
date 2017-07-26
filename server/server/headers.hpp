#ifndef headers_hpp
#define headers_hpp

#include <string>
#include <unordered_map>

class Response;

class Headers {
    
    std::unordered_map<std::string, std::string> headers;
    
public:
    
    void set(std::string, std::string);
    std::string get(std::string);
    
    bool write(Response*);
    
};

#endif
