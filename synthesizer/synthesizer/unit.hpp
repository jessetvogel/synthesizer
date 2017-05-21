#ifndef unit_hpp
#define unit_hpp

#include <string>

class Controller;
class Instrument;

class Unit {
    
private:
    
    bool updated;
    bool constant = false;
    
protected:
    
    Controller* controller;
    bool keyDependent;
    bool applyAlways = false;
    
    virtual void apply(Instrument*);
    
public:
    
    float* output;
    
    virtual bool setValue(std::string, std::string);
    
    void reset();
    void update(Instrument*);
    
    bool isKeyDependent();
    
    static Unit* create(Controller*, std::string, bool);
    static bool set(Controller*, Unit**, std::string, bool);
    
};

#endif
