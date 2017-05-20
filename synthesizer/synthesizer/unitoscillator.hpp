#ifndef unitoscillator_hpp
#define unitoscillator_hpp

#include "unit.hpp"

class Sample;

class UnitOscillator : public Unit {
    
    Sample* sample;
    Unit* frequency = NULL;
    Unit* amplitude = NULL;
    Unit* mean = NULL;
    
    double* phase;
    
public:
    
    UnitOscillator(Controller*, bool);
    ~UnitOscillator();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
