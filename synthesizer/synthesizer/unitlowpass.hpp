#ifndef unitlowpass_hpp
#define unitlowpass_hpp

#include "unit.hpp"

class IIRFilter;

class UnitLowpass : public Unit {

    Parameter* input;
    Parameter* cutOff;
    
    int order;
    double omegaC;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    UnitLowpass(Controller*, int);
    ~UnitLowpass();
    
    void apply(Instrument*);
    
    static const int maxOrder;
    
};

#endif
