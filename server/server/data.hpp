#ifndef data_hpp
#define data_hpp

#include <regex>

#include "request.hpp"
#include "response.hpp"

class Data {

    static std::regex regexSettingsStore;
    static std::regex regexInstrumentData;
    static std::regex regexInstrumentStore;
    
public:

    static bool handle(Request*, Response*);
    static bool store(std::string, std::string);

};

#endif
