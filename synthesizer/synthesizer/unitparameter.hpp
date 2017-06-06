#ifndef unitparameter_hpp
#define unitparameter_hpp

#include "unit.hpp"
#include "interpolation.hpp"

class UnitParameter : public Unit {
    
    double value;
    Unit* min = NULL;
    Unit* max = NULL;
    Interpolation::Type curve;
    
public:
    
    int MidiCCNumber;
    
    UnitParameter(Controller*, int);
    ~UnitParameter();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
    void setValue(double);
};


#endif
