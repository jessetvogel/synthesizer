#ifndef nodefunction_hpp
#define nodefunction_hpp

#include "node.hpp"
#include "function.hpp"

class NodeFunction : public Node {
    
    NodeInput* input;
    NodeInput* function;
    
    NodeOutput* output;
    
public:
    
    NodeFunction(Controller*, Arguments);
    
    void apply();
    
};

#endif
