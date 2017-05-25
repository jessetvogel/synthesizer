#ifndef unitleadkeyfrequency_hpp
#define unitleadkeyfrequency_hpp

#include "unit.hpp"

class UnitLeadKeyFrequency : public Unit {
    
public:
    
    UnitLeadKeyFrequency(Controller*);
    ~UnitLeadKeyFrequency();
    
    void apply(Instrument*);
};


#endif
