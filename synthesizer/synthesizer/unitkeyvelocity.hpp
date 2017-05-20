#ifndef unitkeyvelocity_hpp
#define unitkeyvelocity_hpp

#include "unit.hpp"

class UnitKeyVelocity : public Unit {
    
public:
    
    UnitKeyVelocity(Controller*);
    ~UnitKeyVelocity();
    
    void apply(Instrument*);
};

#endif
