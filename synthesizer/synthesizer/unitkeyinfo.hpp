#ifndef unitkeyinfo_hpp
#define unitkeyinfo_hpp

#include "unit.hpp"

class UnitKeyInfo : public Unit {

public:
    
    enum Type { Frequency, Velocity, Duration, Release };
    
private:
    
    Type type;
    
public:
    
    UnitKeyInfo(Controller*, Type);
    ~UnitKeyInfo();
    
    void apply(Instrument*);
};

#endif
