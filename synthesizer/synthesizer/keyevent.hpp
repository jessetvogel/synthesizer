#ifndef keyevent_hpp
#define keyevent_hpp

#include "settings.hpp"

class KeyEvent {
    
    static bool idUsed[SETTINGS_POLYPHONY];
    
public:

    KeyEvent();
    ~KeyEvent();
    
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
