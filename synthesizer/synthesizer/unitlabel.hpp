#ifndef unitlabel_hpp
#define unitlabel_hpp

#include "unit.hpp"

class UnitLabel : public Unit {
    
    Parameter* value;
    
public:
    
    UnitLabel(Controller*);
    
    void apply(Instrument*);
    
};

#endif
