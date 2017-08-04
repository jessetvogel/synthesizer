#ifndef nodelowpass_hpp
#define nodelowpass_hpp

#include "node.hpp"

class IIRFilter;

class NodeLowpass : public Node {

    NodeInput* input;
    NodeInput* cutOff;
    
    NodeOutput* output;
    
    int order;
    
    int voices;
    double* omegaC;
    IIRFilter** filters;
    
    void updateFilter(int);
    
public:
    
    NodeLowpass(Controller*, Options);
    ~NodeLowpass();
    
    void apply();
    
    static const int maxOrder;
    
};

#endif
