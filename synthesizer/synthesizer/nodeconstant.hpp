#ifndef nodeconstant_hpp
#define nodeconstant_hpp

#include "node.hpp"

class NodeConstant : public Node {
    
    NodeOutput* output;
    
    double value;
    
public:
    
    NodeConstant(Controller*, double);
    
    inline NodeOutput* getOutput() { return output; }
    
    inline void apply() {};
};

#endif
