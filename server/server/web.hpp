#ifndef web_hpp
#define web_hpp

#include <regex>

#include "request.hpp"
#include "response.hpp"

class Web {

    static std::regex regexWeb;
    static std::regex regexExtension;
    
public:
    
    static bool sendFile(Request*, Response*, std::string);
    
    static bool handle(Request*, Response*);
    
};

#endif
