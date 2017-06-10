#ifndef unitlowpass_hpp
#define unitlowpass_hpp

#include "unit.hpp"

class UnitLowpass : public Unit {

    Unit* input = NULL;
    Unit* cutOffFrequency = NULL;
    
    float output_1;
    
public:
    
    UnitLowpass(Controller*);
    ~UnitLowpass();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
