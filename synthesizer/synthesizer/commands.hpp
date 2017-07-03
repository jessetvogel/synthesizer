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
    
    // Commands for MIDI
    static std::regex regexMidiAddDevice;
    static std::regex regexMidiRemoveDevice;
    
    // Commands for audio
    static std::regex regexAudioSetInputDevice;
    static std::regex regexAudioSetOutputDevice;
    
    // Commands for settings
    static std::regex regexSetSampleRate;
    static std::regex regexSetBufferSize;
    static std::regex regexSetRootDirectory;
    
    // Commands for options
    static std::regex regexSetSustainPedalPolarity;
    static std::regex regexSetPitchWheelRange;
    
    // Commands for controller
    static std::regex regexStart;
    static std::regex regexStop;
    static std::regex regexReset;
    
    // Commands for status
    static std::regex regexStatus;
    
    // Commands for synths
    static std::regex regexInclude;
    
    // Commands for instruments
    static std::regex regexInstrumentCreate;
    static std::regex regexInstrumentDelete;
    static std::regex regexInstrumentSetActive;
    static std::regex regexInstrumentSetOutput;
    static std::regex regexInstrumentSetKeyOutput;
    static std::regex regexInstrumentSetKeyReleaseTime;
    
    // Commands for units
    static std::regex regexUnitCreate;
    static std::regex regexUnitKeyCreate;
    static std::regex regexUnitRename;
    static std::regex regexUnitDelete;
    static std::regex regexUnitSetValue;
    
    // Commands for blocks
    static std::regex regexBlockCreate;
    static std::regex regexBlockDelete;
    static std::regex regexBlockAddInput;
    static std::regex regexBlockAddOutput;
    static std::regex regexBlockAttachUnit;

};

#endif
