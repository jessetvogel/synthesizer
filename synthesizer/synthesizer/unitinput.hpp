#ifndef unitinput_hpp
#define unitinput_hpp

#include "unit.hpp"

class UnitInput : public Unit {
    
public:
    
    UnitInput(Controller*);
    
    void apply(Instrument*);
    
};


#endif
