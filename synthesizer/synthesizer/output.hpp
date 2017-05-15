#ifndef output_hpp
#define output_hpp

#include "controller.hpp"
#include <portaudio.h>

class Controller;

class Output {

    Controller* controller;
    
    PaStream* outputStream;
    
    bool active;
    int outputDevice;
    double sampleRate;
    unsigned long framesPerBuffer;
    
    static int callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
    
public:
    
    Output(Controller*);
    int amountOfDevices();
    const char* deviceName(int);
    bool isOutput(int);
    
    void setOutputDevice(int);
    void setSampleRate(double);
    void setFramesPerBuffer(unsigned long);
    
    void start();
    void stop();
    
};

#endif
