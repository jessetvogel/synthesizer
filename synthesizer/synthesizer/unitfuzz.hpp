#ifndef unitfuzz_hpp
#define unitfuzz_hpp

#include "unit.hpp"

class UnitFuzz : public Unit {
    
    Unit* input = NULL;
    Unit* inputGain = NULL;
    
public:
    
    UnitFuzz(Controller*, bool);
    ~UnitFuzz();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
