#ifndef settings_hpp
#define settings_hpp

#define AMOUNT_OF_KEYS (128)
#define NOTE_A4 (69)

#define DEFAULT_SAMPLE_RATE (44100)
#define DEFAULT_BUFFER_SIZE (64)

#define DEFAULT_PITCH_WHEEL_RANGE (2.0)
#define DEFAULT_SUSTAIN_PEDAL_POLARITY (false)

class Settings {
   
public:
    
    Settings();
    
    double frequencies[AMOUNT_OF_KEYS];
    
    double sampleRate;
    unsigned long bufferSize;
    
    double pitchWheelRange;
    bool sustainPedalPolarity;
    
};

#endif
