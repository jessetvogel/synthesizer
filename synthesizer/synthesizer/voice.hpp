#ifndef voice_hpp
#define voice_hpp

#include "settings.hpp"

class Voice {
    
    static bool idUsed[SETTINGS_MAX_VOICES];
    
public:

    Voice();
    ~Voice();
    
    enum Stage { Off, Press, Sustain, Released };
    
    int id;
    
    unsigned char key;
    Stage stage;
    double velocity;
    double frequency;
    double duration;
    double release;
    
};

#endif
