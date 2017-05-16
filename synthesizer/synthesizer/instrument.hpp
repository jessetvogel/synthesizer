#ifndef instrument_hpp
#define instrument_hpp

#include "midistate.hpp"
#include "controller.hpp"
#include "sample.hpp"

class Controller;

class Instrument {
  
    Controller* controller;

    Sample* sample;
    
    float* buffer;
    void updateNote(MidiState*, int);
    double* phase;
    
public:
    
    Instrument(Controller*);
    ~Instrument();
    
    void setSample(Sample*);
    
    void update(MidiState*);
    void addBuffer(float*);
    
    
    
};

#endif
