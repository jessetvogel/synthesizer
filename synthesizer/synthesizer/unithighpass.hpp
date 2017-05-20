#ifndef unithighpass_hpp
#define unithighpass_hpp

#include "unit.hpp"

class UnitHighpass : public Unit {
    
    Unit* input = NULL;
    Unit* cutOffFrequency = NULL;
    
    float* lastOutput;
    float* lastInputOutput;
    
public:
    
    UnitHighpass(Controller*, bool);
    ~UnitHighpass();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
