#ifndef nodeconditional_hpp
#define nodeconditional_hpp

#include "node.hpp"

class NodeConditional : public Node {
    
    NodeInput* input;
    NodeInput* low;
    NodeInput* high;
    NodeInput* outputLow;
    NodeInput* outputMiddle;
    NodeInput* outputHigh;
    
    NodeOutput* output;
    
public:
    
    NodeConditional(Controller*, Arguments);
    
    void apply();

};


#endif
