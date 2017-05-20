#ifndef commands_hpp
#define commands_hpp

#include <regex>

// For macOS/Unix
#define REGEX_DIRECTORY_SEPARATOR "\\/"
#define DIRECTORY_SEPARATOR "/"
// For Windows
//#define REGEX_DIRECTORY_SEPARATOR "\\\\"
//#define DIRECTORY_SEPARATOR "\\"

class Commands {

public:
    
    static std::regex regexInclude;
    
    static std::regex regexInstrumentCreate;
    static std::regex regexInstrumentDelete;
    static std::regex regexInstrumentSetOutput;
    static std::regex regexInstrumentSetKeyOutput;
    static std::regex regexInstrumentSetKeyReleaseTime;
    
    static std::regex regexUnitCreate;
    static std::regex regexUnitKeyCreate;
    static std::regex regexUnitDelete;
    static std::regex regexUnitSetValue;
    
    
    
    // TODO: remove this
    static std::regex regexPrint;
};

#endif
