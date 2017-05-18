#ifndef ADSR_hpp
#define ADSR_hpp

#include "keyunit.hpp"
#include "interpolation.hpp"

class ADSR : public KeyUnit {
    
    double attackTime;
    double decayTime;
    double sustainLevel;
    double releaseTime;
    
    Interpolation::Type attackType;
    Interpolation::Type decayType;
    Interpolation::Type releaseType;
    
    KeyUnit* input;
    
public:
    
    ADSR(Controller*);
    ~ADSR();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
