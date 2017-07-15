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
    static std::regex regexSettingsSet;
    
    // Commands for options
    static std::regex regexSetSustainPedalPolarity;
    static std::regex regexSetPitchWheelRange;
    
    // Commands for controller
    static std::regex regexPlay;
    static std::regex regexReset;
    
    // Commands for status
    static std::regex regexStatus;
    
    // Commands for synths
    static std::regex regexInclude;
        
    // Commands for nodes
    static std::regex regexNodeCreate;
    static std::regex regexNodeKeyCreate;
    static std::regex regexNodeRename;
    static std::regex regexNodeDelete;
    static std::regex regexNodeHide;
    static std::regex regexNodeSet;
    
    // Commands for custom nodes
    static std::regex regexNodeAddInput;
    static std::regex regexNodeAddOutput;
    static std::regex regexNodeAttach;

};

#endif
