#ifndef status_hpp
#define status_hpp

#include <string>

class Controller;

class Status {

    Controller* controller;
    
public:
    
    Status(Controller*);
    
    bool print(std::string);
    
};

#endif
