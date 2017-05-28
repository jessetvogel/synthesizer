#ifndef output_hpp
#define output_hpp

#include <portaudio.h>

class Controller;

class Output {

    Controller* controller;
    
    PaStream* outputStream;
    
    bool active;
    
    static int callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
    
public:
    
    Output(Controller*);
//    ~Output();
    
    bool start();
    bool stop();
    
    int amountOfDevices();
    const char* deviceName(int);
    bool isOutput(int);
    
};

#endif
