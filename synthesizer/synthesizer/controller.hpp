#ifndef controller_hpp
#define controller_hpp

#include <vector>
#include "input.hpp"
#include "output.hpp"
#include "midistate.hpp"
#include "settings.hpp"
#include "instrument.hpp"

class Input;
class Output;
class Instrument;

class Controller {
    
    Input* input;
    Output* output;
    MidiState* midiState;
    Settings* settings;
    
    std::vector<Instrument*> instruments;
    
    int inputDevice;
    int outputDevice;
    
    double sampleRate;
    unsigned long framesPerBuffer;
    
    bool active;
    
    float* buffer;
    
public:
    
    Controller();
    ~Controller();
    
    void start();
    void stop();
    
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
    
    void addInstrument(Instrument*);
};

#endif
