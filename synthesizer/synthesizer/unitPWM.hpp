#ifndef unitPWM_hpp
#define unitPWM_hpp

#include "unit.hpp"

class UnitPWM : public Unit {
    
    Parameter* frequency;
    Parameter* duty;
    Parameter* high;
    Parameter* low;
    
    double* phase;
    
public:
    
    UnitPWM(Controller*, Arguments);
    ~UnitPWM();
    
    void apply(Instrument*);
    
};

#endif
