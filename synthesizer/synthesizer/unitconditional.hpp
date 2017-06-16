#ifndef unitconditional_hpp
#define unitconditional_hpp

#include "unit.hpp"

class UnitConditional : public Unit {
    
    Parameter* input;
    Parameter* low;
    Parameter* high;
    Parameter* outputLow;
    Parameter* outputMiddle;
    Parameter* outputHigh;
    
public:
    
    UnitConditional(Controller*, bool);
    
    void apply(Instrument*);

};


#endif
