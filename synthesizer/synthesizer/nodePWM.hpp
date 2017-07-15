#ifndef nodePWM_hpp
#define nodePWM_hpp

#include "node.hpp"

class NodePWM : public Node {
    
    NodeInput* frequency;
    NodeInput* duty;
    NodeInput* high;
    NodeInput* low;
    
    NodeOutput* output;
    
    double* phase;
    
public:
    
    NodePWM(Controller*, Arguments);
    ~NodePWM();
    
    void apply();
    
};

#endif
