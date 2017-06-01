#ifndef unitdelay_hpp
#define unitdelay_hpp

#include "unit.hpp"

class UnitDelay : public Unit {
    
    int n;
    Unit* input = NULL;
    Unit* gain = NULL;
    Unit* feedback = NULL;
    Unit** times;
    Unit** gains;
    
    float* memory;
    int memoryLength;
    
public:
    
    UnitDelay(Controller*, int, double);
    ~UnitDelay();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
    static const int maxN;
    static const double maxT;
};

#endif
