#ifndef voice_hpp
#define voice_hpp

#include "settings.hpp"

class Voice {
    
public:

    Voice();
    
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
