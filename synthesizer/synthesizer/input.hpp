#ifndef input_hpp
#define input_hpp

#include <portmidi.h>
#include "controller.hpp"

class Controller;

class Input {
    
    Controller* controller;

    PmStream* inputStream;
    
    bool active;
    int inputDevice;

public:
    
    Input(Controller*);
    int amountOfDevices();
    const char* deviceName(int);
    bool isInput(int);
    
    void setInputDevice(int);
    
    void start();
    void update();
    void stop();
    
};

#endif
