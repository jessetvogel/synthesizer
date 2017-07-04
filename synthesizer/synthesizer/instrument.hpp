#ifndef instrument_hpp
#define instrument_hpp

#include <vector>
#include <string>
#include "keyevent.hpp"

#define INSTRUMENT_DEFAULT_ID "#undefined"

class Controller;
class Parameter;

class Instrument {
  
public:
    
private:
    
    Controller* controller;
    
    std::string id = INSTRUMENT_DEFAULT_ID;
    
    Parameter* keyOutput;
    Parameter* output;
    
    std::vector<KeyEvent*> keyEvents;
    
    float* buffer;
    float* keyBuffer;
    
public:
    
    KeyEvent* currentKey;
    
    Instrument(Controller*);
    ~Instrument();
    
    bool active;
    double keyReleaseTime;
    
    bool setId(std::string);
    inline std::string getId() { return id; }
    
    bool setKeyReleaseTime(double);
    
    inline Parameter* getOutput() { return output; };
    inline Parameter* getKeyOutput() { return keyOutput; };
    
    void addKeyEvent(KeyEvent*);
    
    bool update();
    inline float* getBuffer() { return buffer; }
    inline float* getKeyBuffer() { return keyBuffer; }
};

#endif
