#ifndef keyfrequency_hpp
#define keyfrequency_hpp

#include "keyunit.hpp"

class KeyFrequency : public KeyUnit {

public:
    
    KeyFrequency(Controller*);
    ~KeyFrequency();
    
    void apply(Instrument*);
};

#endif
