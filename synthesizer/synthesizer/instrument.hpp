#ifndef instrument_hpp
#define instrument_hpp

#include <vector>
#include "keyevent.hpp"

class Controller;
class Unit;

class Instrument {
  
public:
    
private:
    
    Controller* controller;
    
    Unit* keyOutput = NULL;
    Unit* output = NULL;
    
    std::vector<KeyEvent*> keyEvents;
    
    float* buffer;
    float* keyBuffer;
    
    double keyReleaseTime;
    
public:
    
    KeyEvent* currentKey;
    
    Instrument(Controller*);
    ~Instrument();
    
    bool setOutput(Unit*);
    bool setKeyOutput(Unit*);
    bool setKeyReleaseTime(double);
    
    Unit* getOutput();
    Unit* getKeyOutput();
    
    void addKeyEvent(KeyEvent*);
    
    void update();
    float* getBuffer();
    float* getKeyBuffer();
};

#endif
