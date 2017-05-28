#ifndef unitleadkeyinfo_hpp
#define unitleadkeyinfo_hpp

#include "unit.hpp"

class UnitLeadKeyInfo : public Unit {
    
public:

    enum Type { Frequency, Velocity, Duration, Release };
    
private:
    
    Type type;
    
public:
    
    UnitLeadKeyInfo(Controller*, Type);
    ~UnitLeadKeyInfo();
    
    void apply(Instrument*);
};


#endif
