#ifndef input_hpp
#define input_hpp

#include <portmidi.h>

class Controller;

class Input {
    
    Controller* controller;

    PmStream* inputStream;
    
    int inputDevice;
    bool active;

public:
    
    Input(Controller*, int);
//    ~Input();
    
    bool start();
    bool stop();
    bool update();

    int getInputDevice();
    
    static int amountOfDevices();
    static const char* deviceName(int);
    static bool isInput(int);
    
};

#endif
