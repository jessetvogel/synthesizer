#ifndef status_hpp
#define status_hpp

class Controller;

class Status {

    Controller* controller;
    
    void printInputDevices();
    void printOutputDevices();
    
public:
    
    Status(Controller*);
    
    void print();
    
};

#endif
