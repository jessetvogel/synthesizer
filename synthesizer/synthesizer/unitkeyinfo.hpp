#ifndef unitkeyinfo_hpp
#define unitkeyinfo_hpp

#include "unit.hpp"

class UnitKeyInfo : public Unit {

public:
    
    enum InfoType { Frequency, Velocity, Duration, Release };
    
private:
    
    InfoType infoType;
    
public:
    
    UnitKeyInfo(Controller*, InfoType);
    
    void apply();
    
};

#endif
