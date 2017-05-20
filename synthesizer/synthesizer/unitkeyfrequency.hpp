#ifndef unitkeyfrequency_hpp
#define unitkeyfrequency_hpp

#include "unit.hpp"

class UnitKeyFrequency : public Unit {

public:
    
    UnitKeyFrequency(Controller*);
    ~UnitKeyFrequency();
    
    void apply(Instrument*);
};

#endif
