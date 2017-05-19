#ifndef keyunitadder_hpp
#define keyunitadder_hpp

#include "keyunit.hpp"

class KeyUnitAdder : public KeyUnit {
    
    int n;
    KeyUnit** gains;
    KeyUnit** inputs;
    
public:
    
    KeyUnitAdder(Controller*, int);
    ~KeyUnitAdder();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
    static const int maxN;
};

#endif
