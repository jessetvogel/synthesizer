#ifndef instrument_hpp
#define instrument_hpp

class Controller;
class Unit;
class MidiState;

class Instrument {
  
public:

    enum Stage { Off, Press, Sustain, Released };
    
private:
    
    Controller* controller;
    
    Unit* keyOutput;
    Unit* output;
    
    float* buffer;
    
    Stage* stage;
    double* velocity;
    double* duration;
    double* release;
    
    double keyReleaseTime;
    
    void updateNote(MidiState*, int);
    
public:
    
    Stage currentStage;
    double currentVelocity;
    double currentDuration;
    double currentRelease;
    double currentFrequency;
    int currentKey;
    
    Instrument(Controller*);
    ~Instrument();
    
    bool setOutput(Unit*);
    bool setKeyOutput(Unit*);
    bool setKeyReleaseTime(double);
    
    void update(MidiState*);
    void addBuffer(float*);
    
};

#endif
