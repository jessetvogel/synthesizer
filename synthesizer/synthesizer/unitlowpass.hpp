#ifndef unitlowpass_hpp
#define unitlowpass_hpp

#include "unit.hpp"

class UnitLowpass : public Unit {

    Unit* input = NULL;
    Unit* cutOffFrequency = NULL;
    
    float* lastOutput;
    
public:
    
    UnitLowpass(Controller*, bool);
    ~UnitLowpass();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
