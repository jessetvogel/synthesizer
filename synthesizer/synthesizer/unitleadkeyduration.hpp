#ifndef unitleadkeyduration_hpp
#define unitleadkeyduration_hpp

#include "unit.hpp"

class UnitLeadKeyDuration : public Unit {
    
public:
    
    UnitLeadKeyDuration(Controller*);
    ~UnitLeadKeyDuration();
    
    void apply(Instrument*);
};

#endif
