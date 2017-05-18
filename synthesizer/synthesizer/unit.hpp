#ifndef unit_hpp
#define unit_hpp

#include <string>
#include "controller.hpp"

class Unit {

private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    virtual void apply();
    
public:
    
    float* output;
    
    virtual bool setValue(std::string, std::string);
    
    void reset();
    void update();
    
    static Unit* create(Controller*, std::string);
};

#endif
