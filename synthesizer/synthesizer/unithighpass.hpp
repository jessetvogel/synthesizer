#ifndef unithighpass_hpp
#define unithighpass_hpp

#include "unit.hpp"

class UnitHighpass : public Unit {
    
    Parameter* input;
    Parameter* cutOffFrequency;
    
    float output_1;
    float input_1;
    
public:
    
    UnitHighpass(Controller*);
    
    void apply(Instrument*);
    
};

#endif
