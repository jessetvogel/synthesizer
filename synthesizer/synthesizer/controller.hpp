#ifndef controller_hpp
#define controller_hpp

class Settings;

class MIDIDevices;
class AudioDevices;

class MidiState;

class Instruments;
class Units;

class Controller {
    
    Settings* settings;
    
    MIDIDevices* midiDevices;
    AudioDevices* audioDevices;
    
    MidiState* midiState;
    
    Instruments* instruments;
    Units* units;
    
    bool active;
    
    float* bufferInput;
    float* bufferOutput;
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    inline Settings* getSettings() { return settings; };
    
    inline MIDIDevices* getMIDIDevices() { return midiDevices; };
    inline AudioDevices* getAudioDevices() { return audioDevices; };

    inline MidiState* getMidiState() { return midiState; };
    
    inline Instruments* getInstruments() { return instruments; };
    inline Units* getUnits() { return units; };

    inline float* getBufferInput() { return bufferInput; };
    inline float* getBufferOutput() { return bufferOutput; };
    
    bool start();
    bool stop();
    bool update();
    
};

#endif
