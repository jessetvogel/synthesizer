#ifndef controller_hpp
#define controller_hpp

#include <mutex>
#include <string>

class Settings;
class MIDIDevices;
class AudioDevices;
class MidiState;
class Nodes;
class Monitor;

class Controller {
    
    Settings* settings;
    MIDIDevices* midiDevices;
    AudioDevices* audioDevices;
    MidiState* midiState;
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
    inline MidiState* getMidiState() { return midiState; }
    inline Nodes* getNodes() { return nodes; }
    inline Monitor* getMonitor() { return monitor; }

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
