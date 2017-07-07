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
    
    int midiCC;
    
    UnitParameter(Controller*, Arguments);
    
    void apply();
    void setValue(double);
};


#endif
