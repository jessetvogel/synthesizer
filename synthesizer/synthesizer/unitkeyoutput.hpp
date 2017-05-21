#ifndef unitkeyoutput_hpp
#define unitkeyoutput_hpp

#include "unit.hpp"

class UnitKeyOutput : public Unit {
    
public:
    
    UnitKeyOutput(Controller*);
    ~UnitKeyOutput();
    
    void apply(Instrument*);
};

#endif
