#ifndef unitkeyduration_hpp
#define unitkeyduration_hpp

#include "unit.hpp"

class UnitKeyDuration : public Unit {
    
public:
    
    UnitKeyDuration(Controller*);
    ~UnitKeyDuration();
    
    void apply(Instrument*);
};

#endif
