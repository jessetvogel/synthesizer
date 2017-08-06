#ifndef nodecollectorpoly_hpp
#define nodecollectorpoly_hpp

#include <vector>

#include "nodecollector.hpp"

class Voice;

class NodeCollectorPoly : public NodeCollector {
    
    NodeInput* releaseTime;
    
    std::vector<Voice*> voices;
    bool* idUsed;
    
public:
    
    NodeCollectorPoly(Controller*, Options);
    ~NodeCollectorPoly();
    
    void addKeyEvent(KeyEvent*);
    Voice* createVoice();
    void apply();
    
};

#endif
