#ifndef unitleadkeyvelocity_hpp
#define unitleadkeyvelocity_hpp

#include "unit.hpp"

class UnitLeadKeyVelocity : public Unit {
    
public:
    
    UnitLeadKeyVelocity(Controller*);
    ~UnitLeadKeyVelocity();
    
    void apply(Instrument*);
};

#endif
