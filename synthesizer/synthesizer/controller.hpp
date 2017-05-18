// TODO: maybe put all header files into one single file, so that we don't get problems with references etc.

#ifndef controller_hpp
#define controller_hpp

#include <vector>
#include <string>
#include <unordered_map>

class MidiState;
class Settings;
class Input;
class Output;
class Instrument;
class Unit;
class KeyUnit;

class Controller {
    
    Input* input;
    Output* output;
    MidiState* midiState;
    Settings* settings;
    
    std::unordered_map<std::string, Instrument*> instruments;
    std::unordered_map<std::string, Unit*> units;
    std::unordered_map<std::string, KeyUnit*> keyUnits;
    
    int inputDevice;
    int outputDevice;
    
    double sampleRate;
    unsigned long framesPerBuffer;
    
    bool active;
    
    float* buffer;
    
public:
    
    Controller();
    ~Controller();
    
    bool start();
    bool stop();
    
    float* update();
    
    MidiState* getMidiState();
    Settings* getSettings();
    
    bool setInputDevice(int);
    bool setOutputDevice(int);
    
    bool setSampleRate(double);
    bool setFramesPerBuffer(unsigned long);
    
    void listInputDevices();
    void listOutputDevices();
    
    int getInputDevice();
    int getOutputDevice();
    
    double getSampleRate();
    unsigned long getFramesPerBuffer();
    
    bool addInstrument(Instrument*, std::string);
    bool addUnit(Unit*, std::string);
    bool addKeyUnit(KeyUnit*, std::string);
    
    bool deleteInstrument(std::string);
    bool deleteUnit(std::string);
    bool deleteKeyUnit(std::string);
    
    Instrument* getInstrument(std::string);
    Unit* getUnit(std::string);
    KeyUnit* getKeyUnit(std::string);
    
    void resetUnits();
    void resetKeyUnits();
};

#endif
