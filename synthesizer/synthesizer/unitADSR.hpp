#ifndef unitADSR_hpp
#define unitADSR_hpp

#include "unit.hpp"
#include "interpolation.hpp"

class Unit;

class UnitADSR : public Unit {

    Unit* startLevel = NULL;
    Unit* attackLevel = NULL;
    Unit* sustainLevel = NULL;
    Unit* releaseLevel = NULL;
    
    Unit* attackTime = NULL;
    Unit* decayTime = NULL;
    Unit* releaseTime = NULL;
    
    Interpolation::Type attackType;
    Interpolation::Type decayType;
    Interpolation::Type releaseType;
    
public:
    
    UnitADSR(Controller*);
    ~UnitADSR();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
