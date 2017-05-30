#ifndef unitconditional_hpp
#define unitconditional_hpp

#include "unit.hpp"

class UnitConditional : public Unit {
    
    Unit* input = NULL;
    Unit* low = NULL;
    Unit* high = NULL;
    Unit* outputLow = NULL;
    Unit* outputMiddle = NULL;
    Unit* outputHigh = NULL;
    
public:
    
    UnitConditional(Controller*, bool);
    ~UnitConditional();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);

};


#endif
