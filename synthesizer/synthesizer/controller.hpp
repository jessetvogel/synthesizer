#ifndef controller_hpp
#define controller_hpp

class Settings;
class Options;

class MIDIDevices;
class AudioDevices;

class MidiState;

class Instruments;
class Units;
class Blocks;

class Controller {
    
    Settings* settings;
    Options* options;
    
    MIDIDevices* midiDevices;
    AudioDevices* audioDevices;
    
    MidiState* midiState;
    
    Instruments* instruments;
    Units* units;
    Blocks* blocks;
    
    bool active;
    
    float* bufferInput;
    float* bufferOutput;
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    inline Settings* getSettings() { return settings; };
    inline Options* getOptions() { return options; };
    
    inline MIDIDevices* getMIDIDevices() { return midiDevices; };
    inline AudioDevices* getAudioDevices() { return audioDevices; };

    inline MidiState* getMidiState() { return midiState; };
    
    inline Instruments* getInstruments() { return instruments; };
    inline Units* getUnits() { return units; };
    inline Blocks* getBlocks() { return blocks; };

    inline float* getBufferInput() { return bufferInput; };
    inline float* getBufferOutput() { return bufferOutput; };
    
    bool start();
    bool stop();
    bool reset();
    bool update();
    
};

#endif
