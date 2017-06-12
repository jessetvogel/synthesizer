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
class UnitConstant;

class Controller {
    
    std::vector<Input*> inputs;
    MidiState* midiState;
    Settings* settings;
    
    Output* output;
    
    std::unordered_map<std::string, Instrument*> instruments;
    std::unordered_map<std::string, Unit*> units;
    std::unordered_map<int, UnitParameter*> parameters;
    std::vector<UnitConstant*> unitConstants;
    
    double sampleRate;
    unsigned long framesPerBuffer;
    
    bool active;
    
    float* buffer;
    
    void updateSettings();
    
public:
    
    Controller(Settings*);
    ~Controller();
    
    bool start(int);
    bool stop();
    bool reset();
    bool update();

    bool addInputDevice(int);
    bool removeInputDevice(int);
    
    bool setSampleRate(double);
    bool setFramesPerBuffer(unsigned long);
    
    inline MidiState* getMidiState() { return midiState; };
    inline Settings* getSettings() { return settings; };
    inline double getSampleRate() { return sampleRate; };
    inline unsigned long getFramesPerBuffer() { return framesPerBuffer; };

    bool addInstrument(Instrument*, std::string);
    bool addUnit(Unit*, std::string);
    bool addUnitParameter(UnitParameter*, int);
    UnitConstant* createUnitConstant(double);
    
    Instrument* getInstrument(std::string);
    Unit* getUnit(std::string);
    UnitParameter* getUnitParameter(int);
    bool isUnitConstant(Unit*);
    
    bool renameUnit(std::string, std::string);
    
    bool deleteInstrument(std::string);
    bool deleteUnit(std::string);
    bool deleteUnitParameter(int);
    bool deleteUnitConstant(UnitConstant*);
    
    void resetUnits(bool);
    void addKeyEvent(KeyEvent*);
    inline float* getBuffer() { return buffer; };
};

#endif
