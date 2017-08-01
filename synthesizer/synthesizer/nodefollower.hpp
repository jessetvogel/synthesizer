#ifndef nodefollower_hpp
#define nodefollower_hpp

#include "node.hpp"

class NodeFollower : public Node {
    
    NodeInput* input;
    NodeInput* attackTime;
    NodeInput* decayTime;
    
    NodeOutput* output;
    
    double value;
    
public:
    
    NodeFollower(Controller*, Options);
    
    void apply();
    
};


#endif
