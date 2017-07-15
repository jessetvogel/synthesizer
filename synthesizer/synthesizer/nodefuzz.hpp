#ifndef nodefuzz_hpp
#define nodefuzz_hpp

#include "node.hpp"

class NodeFuzz : public Node {
    
    NodeInput* input;
    NodeInput* inputGain;
    
    NodeOutput* output;
    
public:
    
    NodeFuzz(Controller*, Arguments);
    
    void apply();
    
};

#endif
