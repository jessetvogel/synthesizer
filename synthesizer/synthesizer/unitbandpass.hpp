#ifndef unitbandpass_hpp
#define unitbandpass_hpp

#include "unit.hpp"

class UnitBandPass : public Unit {
    
    Parameter* input;
    Parameter* centerFrequency;
    Parameter* qFactor;
    
    float input_1;
    float input_2;
    float output_1;
    float output_2;
    
public:
    
    UnitBandPass(Controller*);
    
    void apply(Instrument*);
    
};

#endif
