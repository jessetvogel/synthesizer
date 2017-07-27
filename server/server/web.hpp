#ifndef web_hpp
#define web_hpp

#include <regex>

#include "request.hpp"
#include "response.hpp"

class Web {

    static std::regex regexPages;
    static std::regex regexGeneralResource;
    static std::regex regexInstrumentResource;

    static std::regex regexExtension;
    
public:
    
    static bool handle(Request*, Response*);
    static bool sendFile(Request*, Response*, std::string);
};

#endif
