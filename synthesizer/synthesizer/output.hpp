#ifndef output_hpp
#define output_hpp

#include <portaudio.h>

class Controller;

class Output {

    Controller* controller;
    
    PaStream* outputStream;
    
    int outputDevice;
    bool active;
    
    static int callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
    
public:
    
    Output(Controller*, int);
//    ~Output();
    
    bool start();
    bool stop();
    
    static int amountOfDevices();
    static const char* deviceName(int);
    static bool isOutput(int);
    
};

#endif
