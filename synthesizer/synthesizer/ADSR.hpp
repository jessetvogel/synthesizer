#ifndef ADSR_hpp
#define ADSR_hpp

#include "envelope.hpp"

class ADSR : public Envelope {
    
public:
    
    ADSR();
    
    double attack;
    double decay;
    float sustain;
    double release;
    
    float value(bool, double, double);
    
};

#endif
