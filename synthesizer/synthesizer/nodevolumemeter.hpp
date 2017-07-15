#ifndef nodevolumemeter_hpp
#define nodevolumemeter_hpp

#include "node.hpp"

class NodeVolumeMeter : public Node {
    
    NodeInput* input;
    
    NodeOutput* output;
    
    float RMSAverage;
    
public:
    
    NodeVolumeMeter(Controller*, Arguments);
    
    void apply();
    
};


#endif
