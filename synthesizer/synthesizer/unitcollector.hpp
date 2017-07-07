#ifndef unitcollector_hpp
#define unitcollector_hpp

#include "unit.hpp"
#include "keyevent.hpp"

class UnitCollector : public Unit {
    
    Parameter* input;
    Parameter* releaseTime;
    
    std::vector<KeyEvent*> keyEvents;
    
public:
    
    UnitCollector(Controller*, Arguments);
    
    void addKeyEvent(KeyEvent*);
    inline std::vector<KeyEvent*>* getKeyEvents() { return &keyEvents; }
    
    double getReleaseTime();
    Unit* getInput();
    
    inline void apply() {}
    
};

#endif
