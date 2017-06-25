#ifndef unitbandpass_hpp
#define unitbandpass_hpp

#include "unit.hpp"

class UnitBandpass : public Unit {
    
    Parameter* input;
    Parameter* centerFrequency;
    Parameter* qFactor;
    
    float input_1;
    float input_2;
    float output_1;
    float output_2;
    
public:
    
    UnitBandpass(Controller*, int);
    
    void apply(Instrument*);
    
    static const int maxOrder;
    
};

#endif
