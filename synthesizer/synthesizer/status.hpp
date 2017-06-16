#ifndef status_hpp
#define status_hpp

#include <string>

class Controller;

class Status {

    Controller* controller;
    
    void printMidiDevices();
    void printOutputDevices();
    void printUnits();

public:
    
    Status(Controller*);
    
    bool print(std::string);
    
};

#endif
