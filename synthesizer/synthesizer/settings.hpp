#ifndef settings_hpp
#define settings_hpp

#define AMOUNT_OF_KEYS (128)
#define SETTINGS_POLYPHONY (256)

#define DEFAULT_SAMPLE_RATE (44100)
#define DEFAULT_BUFFER_SIZE (64)

#define SETTINGS_PATH "settings.txt"

#include <string>

class Settings {
   
    bool load(std::string);
    bool parseLine(std::string);
    
public:
    
    Settings(std::string);
    
    double sampleRate;
    unsigned long bufferSize;
    
};

#endif
