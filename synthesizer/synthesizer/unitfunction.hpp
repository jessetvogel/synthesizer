#ifndef unitfunction_hpp
#define unitfunction_hpp

#include "unit.hpp"
#include "function.hpp"

class UnitFunction : public Unit {
    
    Parameter* input;
    Parameter* function;
    
public:
    
    UnitFunction(Controller*, bool);
    
    void apply(Instrument*);
    
};

#endif
