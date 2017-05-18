#ifndef keyoscillator_hpp
#define keyoscillator_hpp

#include "keyunit.hpp"

class Sample;

class KeyOscillator : public KeyUnit {
    
    Sample* sample;
    KeyUnit* frequency;
    
    double* phase;
    
public:
    
    KeyOscillator(Controller*);
    ~KeyOscillator();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
};

#endif
