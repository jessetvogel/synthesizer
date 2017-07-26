#ifndef nodelowpass_hpp
#define nodelowpass_hpp

#include "node.hpp"

class IIRFilter;

class NodeLowpass : public Node {

    NodeInput* input;
    NodeInput* cutOff;
    
    NodeOutput* output;
    
    int order;
    double omegaC;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    NodeLowpass(Controller*, Options);
    ~NodeLowpass();
    
    void apply();
    
    static const int maxOrder;
    
};

#endif
