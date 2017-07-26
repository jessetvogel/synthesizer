#ifndef nodeadder_hpp
#define nodeadder_hpp

#include "node.hpp"

class NodeAdder : public Node {

    static const int maxN;
    
    int n;
    NodeInput** gains;
    NodeInput** inputs;

    NodeOutput* output;
    
public:
    
    NodeAdder(Controller*, Options);
    ~NodeAdder();
    
    void apply();
    
};

#endif
