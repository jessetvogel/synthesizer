#ifndef instrument_hpp
#define instrument_hpp

class Controller;
class Unit;
class KeyUnit;
class MidiState;

class Instrument {
  
public:

    enum Stage { Off, Press, Sustain, Released };
    
private:
    
    Controller* controller;
    
    KeyUnit* keyOutput;
    Unit* output;
    
    float* buffer;
    
    Stage* stage;
    unsigned char* velocity;
    double* duration;
    double* release;
    
    double releaseTime;
    
    void updateNote(MidiState*, int);
    
public:
    
    Stage currentStage;
    unsigned char currentVelocity;
    double currentDuration;
    double currentRelease;
    double currentFrequency;
    int currentKey;
    
    Instrument(Controller*);
    ~Instrument();
    
    void setReleaseTime(double);
    void setOutput(Unit*);
    void setKeyOutput(KeyUnit*);
    
    void update(MidiState*);
    void addBuffer(float*);
    
};

#endif
