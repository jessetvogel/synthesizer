#ifndef nodeglider_hpp
#define nodeglider_hpp

#include "node.hpp"

class NodeGlider : public Node {
    
    NodeInput* destination;
    NodeInput* doublingTime;
    
    NodeOutput* output;
    
    double DT;
    double value;
    
public:
    
    NodeGlider(Controller*, Options);
    
    void apply();
    
};

#endif
