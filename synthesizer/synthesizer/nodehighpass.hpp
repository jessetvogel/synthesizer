#ifndef nodehighpass_hpp
#define nodehighpass_hpp

#include "node.hpp"

class IIRFilter;

class NodeHighpass : public Node {
    
    NodeInput* input;
    NodeInput* cutOff;
    
    NodeOutput* output; 
    
    int order;
    double omegaC;
    
    IIRFilter* filter;
    
    void updateFilter();
    
public:
    
    NodeHighpass(Controller*, Options);
    ~NodeHighpass();
    
    void apply();
    
    static const int maxOrder;
    
};

#endif
