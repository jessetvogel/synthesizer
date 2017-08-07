#ifndef settings_hpp
#define settings_hpp

#define AMOUNT_OF_KEYS (128)
#define SETTINGS_MAX_VOICES (256)

#define DEFAULT_SAMPLE_RATE (44100)
#define DEFAULT_BUFFER_SIZE (64)
#define DEFAULT_VOICES      (8)

#define NOTE_A4 (69)

#define DEFAULT_PITCH_WHEEL_RANGE (2.0)
#define DEFAULT_SUSTAIN_PEDAL_POLARITY (false)

#define M_2PI (2.0 * M_PI)

#include <string>

class Settings {
   
    bool load(std::string);
    bool parseLine(std::string);
    
public:
    
    Settings(std::string);
    
    // Constant
    double sampleRate;
    unsigned long bufferSize;
    int voices;
    
    // Changeable
    double masterVolume;
    int masterVolumeCC;
    double frequencies[AMOUNT_OF_KEYS];
    double pitchWheelRange;
    bool sustainPedalPolarity;
    std::string rootDirectory;
    
    // Commands
    bool set(std::string, std::string);

    // Status
    void printSettings();
    
};

#endif
