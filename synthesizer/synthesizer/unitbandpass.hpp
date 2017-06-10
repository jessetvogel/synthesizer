#ifndef unitbandpass_hpp
#define unitbandpass_hpp

#include "unit.hpp"

class UnitBandPass : public Unit {
    
    Unit* input = NULL;
    Unit* centerFrequency = NULL;
    Unit* qFactor = NULL;
    
    float input_1;
    float input_2;
    float output_1;
    float output_2;
    
public:
    
    UnitBandPass(Controller*);
    ~UnitBandPass();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
