#ifndef unitoscillator_hpp
#define unitoscillator_hpp

#include "unit.hpp"

class Parameter;

class UnitOscillator : public Unit {
    
    Parameter* sample;
    Parameter* frequency;
    Parameter* amplitude;
    Parameter* mean;
    
    double* phase;
    
public:
    
    UnitOscillator(Controller*, bool);
    ~UnitOscillator();
    
    void apply(Instrument*);
    
};

#endif
