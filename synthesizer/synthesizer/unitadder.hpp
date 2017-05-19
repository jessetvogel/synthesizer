#ifndef unitadder_hpp
#define unitadder_hpp

#include "unit.hpp"

class UnitAdder : public Unit {

    int n;
    Unit** gains;
    Unit** inputs;
    
public:
    
    UnitAdder(Controller*, int);
    ~UnitAdder();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
    static const int maxN;
};

#endif
