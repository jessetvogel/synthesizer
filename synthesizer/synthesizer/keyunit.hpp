#ifndef keyunit_hpp
#define keyunit_hpp

#include <string>

class Controller;
class Instrument;

class KeyUnit {

private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    virtual void apply(Instrument*);
    
public:

    virtual bool deleteOnRemove();
    
    float* output;
    
    virtual bool setValue(std::string, std::string);
    
    void reset();
    void update(Instrument*);
    
    static KeyUnit* create(Controller*, std::string);
    static bool set(Controller*, KeyUnit**, std::string);
};

#endif
