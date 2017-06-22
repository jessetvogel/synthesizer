#ifndef settings_hpp
#define settings_hpp

#define AMOUNT_OF_KEYS (128)

#define DEFAULT_SAMPLE_RATE (44100)
#define DEFAULT_BUFFER_SIZE (64)
#define DEFAULT_ROOT_DIRECTORY "/Users/Jesse/Projects/synthesizer/files"

#define SETTINGS_PATH "settings.txt"

#include <string>

class Settings {
   
    bool load();
    bool parseLine(std::string);
    
public:
    
    Settings();
    
    double sampleRate;
    unsigned long bufferSize;
    
    std::string rootDirectory;
    
};

#endif
