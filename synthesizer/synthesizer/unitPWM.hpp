#ifndef unitPWM_hpp
#define unitPWM_hpp

#include "unit.hpp"

class UnitPWM : public Unit {
    
    Unit* frequency = NULL;
    Unit* duty = NULL;
    Unit* high = NULL;
    Unit* low = NULL;
    
    double* phase;
    
public:
    
    UnitPWM(Controller*, bool);
    ~UnitPWM();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
