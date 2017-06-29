#ifndef unitbandpass_hpp
#define unitbandpass_hpp

#include "unit.hpp"

class IIRFilter;

class UnitBandpass : public Unit {
    
    Parameter* input;
    Parameter* center;
    Parameter* bandwidth;
    
    int order;
    double omegaL;
    double omegaH;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    UnitBandpass(Controller*, int);
    
    void apply(Instrument*);
    
    static const int maxOrder;
    
};

#endif
