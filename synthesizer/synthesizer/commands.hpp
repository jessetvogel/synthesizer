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
    
    // Remove all surrounding whitespace and comments
    static std::regex regexPreprocess;
    
    // Commands for settings
    static std::regex regexAddInputDevice;
    static std::regex regexRemoveInputDevice;
    static std::regex regexSetOutputDevice;
    
    static std::regex regexSetSampleRate;
    static std::regex regexSetBufferSize;
    static std::regex regexSetSustainPedalPolarity;
    static std::regex regexSetPitchWheelRange;
    
    // Commands for controller
    static std::regex regexStart;
    static std::regex regexStop;
    static std::regex regexReset;
    
    // Commands for synths
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
};

#endif
