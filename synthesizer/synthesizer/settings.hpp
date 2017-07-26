#ifndef settings_hpp
#define settings_hpp

#define AMOUNT_OF_KEYS (128)
#define SETTINGS_POLYPHONY (256)

#define DEFAULT_SAMPLE_RATE (44100)
#define DEFAULT_BUFFER_SIZE (64)

#define NOTE_A4 (69)

#define DEFAULT_PITCH_WHEEL_RANGE (2.0)
#define DEFAULT_SUSTAIN_PEDAL_POLARITY (false)

#include <string>

class Settings {
   
    bool load(std::string);
    bool parseLine(std::string);
    
public:
    
    Settings(std::string);
    
    // Constant
    double sampleRate;
    unsigned long bufferSize;
    
    // Changeable
    double frequencies[AMOUNT_OF_KEYS];
    double pitchWheelRange;
    bool sustainPedalPolarity;
    
    // Commands
    bool set(std::string, std::string);

    // Status
    void printSettings();
    
};

#endif
