#ifndef nodebandpass_hpp
#define nodebandpass_hpp

#include "node.hpp"

class IIRFilter;

class NodeBandpass : public Node {
    
    NodeInput* input;
    NodeInput* center;
    NodeInput* bandwidth;
    
    NodeOutput* output;
    
    int order;
    double omegaL;
    double omegaH;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    NodeBandpass(Controller*, Options);
    
    void apply();
    
    static const int maxOrder;
    
};

#endif
