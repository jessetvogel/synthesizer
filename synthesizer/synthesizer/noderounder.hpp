#ifndef noderounder_hpp
#define noderounder_hpp

#include "node.hpp"

class NodeRounder : public Node {
    
    enum Function { Round, Floor, Ceil };
    
    NodeInput* input;
    
    NodeOutput* output;
    
    Function function;
    
public:
    
    NodeRounder(Controller*, Options);
    
    void apply();
    
};


#endif
