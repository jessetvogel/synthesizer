#ifndef unithighpass_hpp
#define unithighpass_hpp

#include "unit.hpp"

class UnitHighpass : public Unit {
    
    Unit* input = NULL;
    Unit* cutOffFrequency = NULL;
    
    float output_1;
    float input_1;
    
public:
    
    UnitHighpass(Controller*);
    ~UnitHighpass();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
