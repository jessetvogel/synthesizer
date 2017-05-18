#ifndef commands_hpp
#define commands_hpp

#include <regex>

class Commands {

public:
    
    static std::regex regexInstrumentCreate;
    static std::regex regexInstrumentDelete;
    static std::regex regexInstrumentSetOutput;
    static std::regex regexInstrumentKeySetOutput;
    static std::regex regexInstrumentSetReleaseTime;
    
    static std::regex regexUnitCreate;
    static std::regex regexUnitDelete;
    static std::regex regexUnitSetValue;
    
    static std::regex regexKeyUnitCreate;
    static std::regex regexKeyUnitDelete;
    static std::regex regexKeyUnitSetValue;
    
};

#endif
