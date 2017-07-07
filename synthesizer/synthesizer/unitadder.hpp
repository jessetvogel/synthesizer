#ifndef unitadder_hpp
#define unitadder_hpp

#include "unit.hpp"

class UnitAdder : public Unit {

    static const int maxN;
    
    int n;
    Parameter** gains;
    Parameter** inputs;
    
public:
    
    UnitAdder(Controller*, Arguments);
    ~UnitAdder();
    
    void apply();
    
};

#endif
