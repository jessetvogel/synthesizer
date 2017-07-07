#ifndef unitaudioinput_hpp
#define unitaudioinput_hpp

#include "unit.hpp"

class UnitAudioInput : public Unit {
    
    int channel;
    
public:
    
    UnitAudioInput(Controller*, Arguments);
    
    void apply();
    
};


#endif
