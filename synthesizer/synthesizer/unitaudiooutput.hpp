#ifndef unitaudiooutput_hpp
#define unitaudiooutput_hpp

#include "unit.hpp"
#include "keyevent.hpp"

class UnitAudioOutput : public Unit {
    
    int channelCount;
    
    Parameter** channels;
    
public:
    
    UnitAudioOutput(Controller*, Arguments);
    ~UnitAudioOutput();
    
    Unit* getChannel(int);
    
    inline void apply() {};
    
};


#endif
