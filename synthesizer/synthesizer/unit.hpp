#ifndef unit_hpp
#define unit_hpp

#include <string>

class Controller;
class Instrument;

class Unit {
    
private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    bool keyDependent;
    bool applyAlways = false;
    
    inline virtual void apply(Instrument*) { };
    
public:
    
    float* output;
    
    inline virtual bool setValue(std::string, std::string) { return false; };
    
    inline void reset() { updated = false; };
    inline void update(Instrument* instrument) { if(updated) return; if(!applyAlways) updated = true; apply(instrument); };
    
    inline bool isKeyDependent() { return keyDependent; };
    
    static Unit* create(Controller*, std::string, bool, std::string, std::string);
    static bool set(Controller*, Unit**, std::string, bool);
    
};

#endif
