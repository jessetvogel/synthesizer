#ifndef nodeparameter_hpp
#define nodeparameter_hpp

#include "node.hpp"
#include "curve.hpp"

class NodeParameter : public Node {
    
    NodeInput* min;
    NodeInput* max;
    NodeInput* curve;
    
    NodeOutput* output;
    
    int midiCC;
    double value;
    
public:
    
    NodeParameter(Controller*, Arguments);

    inline int getMidiCC() { return midiCC; }
    
    void apply();
    void setValue(double);
};


#endif
