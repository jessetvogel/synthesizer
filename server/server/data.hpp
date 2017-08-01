#ifndef data_hpp
#define data_hpp

#include <regex>

#include "request.hpp"
#include "response.hpp"

class Data {

    static std::regex regexSettingsSet;
    static std::regex regexInstrumentSet;
    static std::regex regexInstrumentGet;
    
public:

    static bool handle(Request*, Response*);
    static bool store(std::string, std::string);

};

#endif
