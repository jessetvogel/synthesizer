#ifndef nodedelay_hpp
#define nodedelay_hpp

#include "node.hpp"

class NodeDelay : public Node {
    
    int n;
    
    NodeInput* input;
    NodeInput* gain;
    NodeInput* feedback;
    NodeInput** times;
    NodeInput** gains;
    
    NodeOutput* output;
    
    float* memory;
    int memoryLength;
    
public:
    
    NodeDelay(Controller*, Arguments);
    ~NodeDelay();
    
    void apply();
    
    static const int maxN;
    static const double maxT;
};

#endif
