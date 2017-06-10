#ifndef unitconstant_hpp
#define unitconstant_hpp

#include <vector>

#include "unit.hpp"

class UnitConstant : public Unit {

    static std::vector<UnitConstant*> units;
    
    double value;
    
    UnitConstant(Controller*, double);
    
public:
    
    
    inline void apply(Instrument*) {};
    
    static UnitConstant* create(Controller*, double);
    static bool remove(UnitConstant*);
    static bool isUnitConstant(Unit*);
};

#endif
