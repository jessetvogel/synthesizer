#ifndef audiodevices_hpp
#define audiodevices_hpp

#include <portaudio.h>

class Controller;

class AudioDevices {

    Controller* controller;
    
    PaStream* stream;
    
    int inputDeviceId;
    int outputDeviceId;
    int inputChannelCount;
    int outputChannelCount;
    bool active;
    
    float* bufferInput;
    float* bufferOutput;
    
    static int callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
    
public:
    
    const static int DEVICE_ID_DEFAULT;
    const static int DEVICE_ID_NONE;
    
    AudioDevices(Controller*);
    
    bool start();
    bool stop();
    
    static int amountOfDevices();
    static const char* deviceName(int);
    static bool isInput(int);
    static bool isOutput(int);
    
    inline int getInputChannelCount() { return inputChannelCount; }
    inline int getOutputChannelCount() { return outputChannelCount; }
    
    inline float* getBufferInput() { return bufferInput; }
    inline float* getBufferOutput() { return bufferOutput; }
    
    // Commands
    bool setInputDeviceId(int);
    bool setOutputDeviceId(int);
    
    // Status
    void printInputDevices();
    void printOutputDevices();
    
};

#endif
