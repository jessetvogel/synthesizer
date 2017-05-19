#ifndef keyoscillator_hpp
#define keyoscillator_hpp

#include "keyunit.hpp"

class Sample;

class KeyUnitOscillator : public KeyUnit {
    
    Sample* sample;
    KeyUnit* frequency = NULL;
    KeyUnit* amplitude = NULL;
    KeyUnit* mean = NULL;
    
    double* phase;
    
public:
    
    KeyUnitOscillator(Controller*);
    ~KeyUnitOscillator();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
};

#endif
