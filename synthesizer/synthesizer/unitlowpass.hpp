#ifndef unitlowpass_hpp
#define unitlowpass_hpp

#include "unit.hpp"

class UnitLowpass : public Unit {

    Parameter* input;
    Parameter* cutOffFrequency;
    
    float output_1;
    
public:
    
    UnitLowpass(Controller*);
    
    void apply(Instrument*);
    
};

#endif
