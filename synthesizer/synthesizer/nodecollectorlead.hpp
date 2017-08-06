#ifndef nodecollectorlead_hpp
#define nodecollectorlead_hpp

#include "nodecollector.hpp"

class NodeCollectorLead : public NodeCollector {
    
    NodeInput* releaseTime;
    NodeInput* glide;
    NodeInput* glideType;
    
    Voice* voice;
    
    double glideTimer;
    
public:
    
    NodeCollectorLead(Controller*, Options);
    ~NodeCollectorLead();
  
    void addKeyEvent(KeyEvent*);
    void apply();
    
};

#endif
