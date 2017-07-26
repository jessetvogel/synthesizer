#ifndef nodefollower_hpp
#define nodefollower_hpp

#include "node.hpp"

class NodeFollower : public Node {
    
    NodeInput* input;
    
    NodeOutput* output;
    
    float RMSAverage;
    
public:
    
    NodeFollower(Controller*, Options);
    
    void apply();
    
};


#endif
