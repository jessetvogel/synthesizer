#ifndef nodeADSR_hpp
#define nodeADSR_hpp

#include "node.hpp"

class NodeADSR : public Node {

    NodeInput* startLevel;
    NodeInput* attackLevel;
    NodeInput* sustainLevel;
    NodeInput* releaseLevel;
    
    NodeInput* attackTime;
    NodeInput* decayTime;
    NodeInput* releaseTime;
    
    NodeInput* duration;
    NodeInput* release;
    
    NodeInput* attackCurve;
    NodeInput* decayCurve;
    NodeInput* releaseCurve;
    
    NodeOutput* output;
    
public:
    
    NodeADSR(Controller*, Options);
    
    void apply();
    
};

#endif
