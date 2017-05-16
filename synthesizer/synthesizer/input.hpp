#ifndef input_hpp
#define input_hpp

#include <portmidi.h>
#include "controller.hpp"

class Controller;

class Input {
    
    Controller* controller;

    PmStream* inputStream;
    
    bool active;

public:
    
    Input(Controller*);
//    ~Input();
    
    void start();
    void stop();
    
    void update();
    
    int amountOfDevices();
    const char* deviceName(int);
    bool isInput(int);
    
};

#endif
