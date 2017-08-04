#ifndef nodehighpass_hpp
#define nodehighpass_hpp

#include "node.hpp"

class IIRFilter;

class NodeHighpass : public Node {
    
    NodeInput* input;
    NodeInput* cutOff;
    
    NodeOutput* output; 
    
    int order;
    
    int voices;
    double* omegaC;
    IIRFilter** filters;
    
    void updateFilter(int);
    
public:
    
    NodeHighpass(Controller*, Options);
    ~NodeHighpass();
    
    void apply();
    
    static const int maxOrder;
    
};

#endif
