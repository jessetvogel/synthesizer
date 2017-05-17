#ifndef instrument_hpp
#define instrument_hpp

#include "midistate.hpp"
#include "controller.hpp"
#include "sample.hpp"
#include "envelope.hpp"

class Controller;

class Instrument {
  
    enum Stage { Off, Press, Sustain, Released };
    
    Controller* controller;

    Sample* sample;
    Envelope* envelope;
    
    float* buffer;
    void updateNote(MidiState*, int);
    Stage* stage;
    unsigned char* velocity;
    double* duration;
    double* release;
    double* phase;
    
public:
    
    Instrument(Controller*);
    ~Instrument();
    
    void setSample(Sample*);
    void setEnvelope(Envelope*);
    
    void update(MidiState*);
    void addBuffer(float*);
    
    
    
};

#endif
