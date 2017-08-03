#ifndef nodecollectorlead_hpp
#define nodecollectorlead_hpp

#include "nodecollector.hpp"

class Voice;

class NodeCollectorLead : public NodeCollector {
    
    NodeInput* releaseTime;
    
    Voice* voice;
    
public:
    
    NodeCollectorLead(Controller*, Options);
    ~NodeCollectorLead();
  
    void addKeyEvent(KeyEvent*);
    void apply();
    
    double getReleaseTime();
};

#endif
