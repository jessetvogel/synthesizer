#ifndef controller_hpp
#define controller_hpp

#include <string>

class Settings;
class Options;

class MIDIDevices;
class AudioDevices;

class MidiState;

class Units;
class Blocks;

class Controller {
    
    Settings* settings;
    Options* options;
    
    MIDIDevices* midiDevices;
    AudioDevices* audioDevices;
    
    MidiState* midiState;
    
    Units* units;
    Blocks* blocks;
    
    bool active;
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    inline Settings* getSettings() { return settings; };
    inline Options* getOptions() { return options; };
    
    inline MIDIDevices* getMIDIDevices() { return midiDevices; };
    inline AudioDevices* getAudioDevices() { return audioDevices; };

    inline MidiState* getMidiState() { return midiState; };
    
    inline Units* getUnits() { return units; };
    inline Blocks* getBlocks() { return blocks; };

    bool start();
    bool stop();
    bool reset();
    bool update();
    
    // Commands
    bool play(std::string);

    // Status
    void printState();
    
};

#endif
