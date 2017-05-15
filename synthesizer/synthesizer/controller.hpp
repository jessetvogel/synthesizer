#ifndef controller_hpp
#define controller_hpp

#include "input.hpp"
#include "output.hpp"
#include "midistate.hpp"

class Input;
class Output;

class Controller {

    Input* input;
    Output* output;
    MidiState* midiState;

    bool active;
    
public:
    
    Controller();
    ~Controller();
    
    void start();
    void update();
    void stop();
    
    MidiState* getMidiState();
    
    void setInputDevice(int);
    void setOutputDevice(int);
    
    void listInputDevices();
    void listOutputDevices();
};

#endif
