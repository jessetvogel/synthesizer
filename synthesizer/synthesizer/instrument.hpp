#ifndef instrument_hpp
#define instrument_hpp

#include <vector>
#include <string>
#include "keyevent.hpp"

#define INSTRUMENT_DEFAULT_ID "#undefined"

class Controller;
class Unit;

class Instrument {
  
public:
    
private:
    
    Controller* controller;
    
    std::string id = INSTRUMENT_DEFAULT_ID;
    
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
    
    bool active;

    bool setId(std::string);
    inline std::string getId() { return id; }
    
    bool setOutput(Unit*);
    bool setKeyOutput(Unit*);
    bool setKeyReleaseTime(double);
    
    Unit* getOutput();
    Unit* getKeyOutput();
    
    void addKeyEvent(KeyEvent*);
    
    bool update();
    inline float* getBuffer() { return buffer; }
    inline float* getKeyBuffer() { return keyBuffer; }
};

#endif
