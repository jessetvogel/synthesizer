#ifndef unitfuzz_hpp
#define unitfuzz_hpp

#include "unit.hpp"

class UnitFuzz : public Unit {
    
    Parameter* input = NULL;
    Parameter* inputGain = NULL;
    
public:
    
    UnitFuzz(Controller*, Arguments);
    
    void apply(Instrument*);
    
};

#endif
