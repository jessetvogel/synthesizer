#ifndef nodefuzz_hpp
#define nodefuzz_hpp

#include "node.hpp"

class NodeFuzz : public Node {
    
    NodeInput* input;
    NodeInput* inputGain;
    
    NodeOutput* output;
    
    static double fuzz(double, double);
    
public:
    
    NodeFuzz(Controller*, Options);
    
    void apply();
    
};

#endif
