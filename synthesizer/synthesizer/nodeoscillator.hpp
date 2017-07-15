#ifndef nodeoscillator_hpp
#define nodeoscillator_hpp

#include "node.hpp"

class NodeInput;

class NodeOscillator : public Node {
    
    NodeInput* sample;
    NodeInput* frequency;
    NodeInput* amplitude;
    NodeInput* mean;
    
    NodeOutput* output;
    
    double* phase;
    
public:
    
    NodeOscillator(Controller*, Arguments);
    ~NodeOscillator();
    
    void apply();
    
};

#endif
