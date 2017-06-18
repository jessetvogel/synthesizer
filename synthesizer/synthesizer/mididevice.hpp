#ifndef mididevice_hpp
#define mididevice_hpp

#include <portmidi.h>

class Controller;

class MIDIDevice {
    
    Controller* controller;
    
    PmStream* inputStream;

    int deviceId;
    bool active;

public:
    
    static int DEVICE_ID_DEFAULT;
    
    MIDIDevice(Controller*, int);
    
    bool start();
    bool stop();
    bool update();
    
    inline int getDeviceId() { return deviceId; }
    inline bool isActive() { return active; }
    
};

#endif
