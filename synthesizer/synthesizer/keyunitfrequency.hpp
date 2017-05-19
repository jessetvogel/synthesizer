#ifndef keyfrequency_hpp
#define keyfrequency_hpp

#include "keyunit.hpp"

class KeyUnitFrequency : public KeyUnit {

public:
    
    KeyUnitFrequency(Controller*);
    ~KeyUnitFrequency();
    
    void apply(Instrument*);
};

#endif
