#ifndef unitADSR_hpp
#define unitADSR_hpp

#include "unit.hpp"

class UnitADSR : public Unit {

    Parameter* startLevel;
    Parameter* attackLevel;
    Parameter* sustainLevel;
    Parameter* releaseLevel;
    
    Parameter* attackTime;
    Parameter* decayTime;
    Parameter* releaseTime;
    
    Parameter* duration;
    Parameter* release;
    
    Parameter* attackCurve;
    Parameter* decayCurve;
    Parameter* releaseCurve;
    
public:
    
    UnitADSR(Controller*, Arguments);
    
    void apply(Instrument*);
    
};

#endif
