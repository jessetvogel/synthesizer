#ifndef unitconstant_hpp
#define unitconstant_hpp

#include "unit.hpp"

class UnitConstant : public Unit {

    double value;

public:

    bool deleteOnRemove();
    
    UnitConstant(Controller*, double);
    
    void apply(Instrument*);
};

#endif
