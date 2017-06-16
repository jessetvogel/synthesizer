#ifndef unitmodulationwheel_hpp
#define unitmodulationwheel_hpp

#include "unit.hpp"

class UnitModulationWheel : public Unit {
    
public:
    
    UnitModulationWheel(Controller*);
    
    void apply(Instrument*);
    
};

#endif
