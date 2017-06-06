#ifndef controller_hpp
#define controller_hpp

#include <vector>
#include <string>
#include <unordered_map>

#include "keyevent.hpp"

class MidiState;
class Settings;
class Input;
class Output;
class Instrument;
class Unit;
class UnitParameter;

class Controller {
    
    std::vector<Input*> inputs;
    Output* output;
    MidiState* midiState;
    Settings* settings;
    
    std::unordered_map<std::string, Instrument*> instruments;
    std::unordered_map<std::string, Unit*> units;
    std::unordered_map<int, UnitParameter*> parameters;
    
    int outputDevice;
    double sampleRate;
    unsigned long framesPerBuffer;
    
    bool active;
    
    float* buffer;
    
    void updateSettings();
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    bool start();
    bool stop();
    bool reset();
    bool update();

    bool addInputDevice(int);
    bool removeInputDevice(int);
    
    bool setOutputDevice(int);
    bool setSampleRate(double);
    bool setFramesPerBuffer(unsigned long);
    
    inline MidiState* getMidiState() { return midiState; };
    inline Settings* getSettings() { return settings; };
    inline int getOutputDevice() { return outputDevice; };
    inline double getSampleRate() { return sampleRate; };
    inline unsigned long getFramesPerBuffer() { return framesPerBuffer; };

    bool addInstrument(Instrument*, std::string);
    bool addUnit(Unit*, std::string);
    bool addUnitParameter(UnitParameter*, int);
    
    Instrument* getInstrument(std::string);
    Unit* getUnit(std::string);
    UnitParameter* getUnitParameter(int);
    
    bool deleteInstrument(std::string);
    bool deleteUnit(std::string);
    bool deleteUnitParameter(int);
    
    void resetUnits(bool);
    void addKeyEvent(KeyEvent*);
    inline float* getBuffer() { return buffer; };
    
    
    // TODO: remove this at some point
    void listInputDevices();
    void listOutputDevices();
};

#endif
