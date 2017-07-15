#ifndef nodemodulationwheel_hpp
#define nodemodulationwheel_hpp

#include "node.hpp"

class NodeModulationWheel : public Node {
    
    NodeOutput* output;
    
public:
    
    NodeModulationWheel(Controller*);
    
    void apply();
    
};

#endif
