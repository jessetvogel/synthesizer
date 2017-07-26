#ifndef monitor_hpp
#define monitor_hpp

#include <string>

class Controller;
class NodeOutput;

class Monitor {
    
    Controller* controller;
    NodeOutput* nodeOutput;
    
public:
    
    Monitor(Controller* controller);
    
    // Commands
    bool monitor(std::string);
    
    // Status
    void printMonitor();
    
};

#endif
