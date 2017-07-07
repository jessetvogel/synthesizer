#ifndef unitdelay_hpp
#define unitdelay_hpp

#include "unit.hpp"

class UnitDelay : public Unit {
    
    int n;
    Parameter* input = NULL;
    Parameter* gain = NULL;
    Parameter* feedback = NULL;
    Parameter** times;
    Parameter** gains;
    
    float* memory;
    int memoryLength;
    
public:
    
    UnitDelay(Controller*, Arguments);
    ~UnitDelay();
    
    void apply();
    
    static const int maxN;
    static const double maxT;
};

#endif
