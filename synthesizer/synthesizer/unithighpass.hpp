#ifndef unithighpass_hpp
#define unithighpass_hpp

#include "unit.hpp"

class IIRFilter;

class UnitHighpass : public Unit {
    
    Parameter* input;
    Parameter* cutOffFrequency;
    
    int order;
    double omegaC;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    UnitHighpass(Controller*, int);
    ~UnitHighpass();
    
    void apply(Instrument*);
    
    static const int maxOrder;
    
};

#endif
