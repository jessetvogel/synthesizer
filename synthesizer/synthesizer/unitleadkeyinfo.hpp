#ifndef unitleadkeyinfo_hpp
#define unitleadkeyinfo_hpp

#include "unit.hpp"

class UnitLeadKeyInfo : public Unit {
    
public:

    enum InfoType { Frequency, Velocity, Duration, Release, Pressing };
    
private:
    
    InfoType infoType;
    
public:
    
    UnitLeadKeyInfo(Controller*, InfoType);
    
    void apply();
    
};


#endif
