#ifndef unitadder_hpp
#define unitadder_hpp

#include "unit.hpp"

class UnitAdder : public Unit {

    int n;
    Parameter** gains;
    Parameter** inputs;
    
public:
    
    UnitAdder(Controller*, bool, int);
    ~UnitAdder();
    
    void apply(Instrument*);
    
    static const int maxN;
};

#endif
