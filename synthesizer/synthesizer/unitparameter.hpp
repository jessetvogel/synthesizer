#ifndef unitparameter_hpp
#define unitparameter_hpp

#include "unit.hpp"
#include "curve.hpp"

class UnitParameter : public Unit {
    
    double value;
    Parameter* min;
    Parameter* max;
    Parameter* curve;
    
public:
    
    int MidiCCNumber;
    
    UnitParameter(Controller*, Arguments);
    
    void apply(Instrument*);
    void setValue(double);
};


#endif
