#ifndef nodecollectorpoly_hpp
#define nodecollectorpoly_hpp

#include <vector>

#include "nodecollector.hpp"

class Voice;

class NodeCollectorPoly : public NodeCollector {
    
    NodeInput* releaseTime;
    
    std::vector<Voice*> voices;
    
public:
    
    NodeCollectorPoly(Controller*, Options);
    ~NodeCollectorPoly();
    
    void addKeyEvent(KeyEvent*);
    void apply();
    
    double getReleaseTime();
};

#endif
