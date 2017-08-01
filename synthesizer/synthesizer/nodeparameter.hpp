#ifndef nodeparameter_hpp
#define nodeparameter_hpp

#include "node.hpp"
#include "curve.hpp"

class NodeParameter : public Node {
    
    NodeInput* cc;
    NodeInput* min;
    NodeInput* max;
    NodeInput* curve;
    NodeInput* valueInput;
    
    NodeOutput* output;
    
    int midiCC;
    double value;
    
public:
    
    NodeParameter(Controller*, Options);

    inline int getMidiCC() { return midiCC; }
    
    void apply();
    void setValue(double);
};


#endif
