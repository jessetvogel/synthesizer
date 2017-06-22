#ifndef unitvolumemeter_hpp
#define unitvolumemeter_hpp

#include "unit.hpp"

class UnitVolumeMeter : public Unit {
    
    Parameter* input;
    float RMSAverage;
    
public:
    
    UnitVolumeMeter(Controller*, bool);
    
    void apply(Instrument*);
    
};


#endif
