#ifndef ADSR_hpp
#define ADSR_hpp

#include "keyunit.hpp"
#include "interpolation.hpp"

class KeyUnitADSR : public KeyUnit {
    
    KeyUnit* attackTime = NULL;
    KeyUnit* decayTime = NULL;
    KeyUnit* sustainLevel = NULL;
    KeyUnit* releaseTime = NULL;
    KeyUnit* input = NULL;
    
    Interpolation::Type attackType;
    Interpolation::Type decayType;
    Interpolation::Type releaseType;
    
public:
    
    KeyUnitADSR(Controller*);
    ~KeyUnitADSR();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
