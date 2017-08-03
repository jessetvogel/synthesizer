#ifndef controller_hpp
#define controller_hpp

#include <mutex>
#include <string>

class Settings;
class MIDIDevices;
class AudioDevices;
class KeyState;
class Nodes;
class Monitor;

class Controller {
    
    Settings* settings;
    MIDIDevices* midiDevices;
    AudioDevices* audioDevices;
    KeyState* keyState;
    Nodes* nodes;
    Monitor* monitor;
    
    bool active;
    
    std::mutex mutex;
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    inline Settings* getSettings() { return settings; }
    inline MIDIDevices* getMIDIDevices() { return midiDevices; }
    inline AudioDevices* getAudioDevices() { return audioDevices; }
    inline KeyState* getKeyState() { return keyState; }
    inline Nodes* getNodes() { return nodes; }
    inline Monitor* getMonitor() { return monitor; }

    inline std::mutex* getMutex() { return &mutex; }
    
    bool start();
    bool stop();
    bool clear();
    bool update();
    
    // Commands
    bool play(std::string);

    // Status
    void printState();
    
};

#endif
