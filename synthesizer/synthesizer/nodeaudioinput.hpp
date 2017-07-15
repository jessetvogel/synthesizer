#ifndef nodeaudioinput_hpp
#define nodeaudioinput_hpp

#include "node.hpp"

class NodeAudioInput : public Node {
    
    NodeOutput* output;
    
    int channel;
    
public:
    
    NodeAudioInput(Controller*, Arguments);
    
    void apply();
    
};


#endif
