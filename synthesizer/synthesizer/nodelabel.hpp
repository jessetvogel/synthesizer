#ifndef nodelabel_hpp
#define nodelabel_hpp

#include "node.hpp"

class NodeLabel : public Node {
    
    NodeInput* value;

    NodeOutput* output;

public:
    
    NodeLabel(Controller*, Options);
    
    void apply();
    
};

#endif
