// TODO: maybe put all header files into one single file, so that we don't get problems with references etc.

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

class Controller {
    
    Input* input;
    Output* output;
    MidiState* midiState;
    Settings* settings;
    
    std::unordered_map<std::string, Instrument*> instruments;
    std::unordered_map<std::string, Unit*> units;
    
    int inputDevice;
    int outputDevice;
    double sampleRate;
    unsigned long framesPerBuffer;
    
    bool active;
    
    float* buffer;
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    bool start();
    bool stop();
    bool update();

    bool setInputDevice(int);
    bool setOutputDevice(int);
    bool setSampleRate(double);
    bool setFramesPerBuffer(unsigned long);
    
    inline MidiState* getMidiState() { return midiState; };
    inline Settings* getSettings() { return settings; };
    inline int getInputDevice() { return inputDevice; };
    inline int getOutputDevice() { return outputDevice; };
    inline double getSampleRate() { return sampleRate; };
    inline unsigned long getFramesPerBuffer() { return framesPerBuffer; };

    bool addInstrument(Instrument*, std::string);
    bool addUnit(Unit*, std::string);
    
    Instrument* getInstrument(std::string);
    Unit* getUnit(std::string);
    
    bool deleteInstrument(std::string);
    bool deleteUnit(std::string);
    
    void resetUnits(bool);
    void addKeyEvent(KeyEvent*);
    inline float* getBuffer() { return buffer; };
    
    
    
    // TODO: remove this at some point
    void listInputDevices();
    void listOutputDevices();
};

#endif
