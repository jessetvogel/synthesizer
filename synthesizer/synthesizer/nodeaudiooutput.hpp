#ifndef nodeaudiooutput_hpp
#define nodeaudiooutput_hpp

#include "node.hpp"
#include "keyevent.hpp"

class NodeAudioOutput : public Node {
    
    int channelCount;
    
    NodeInput** channels;
    
public:
    
    NodeAudioOutput(Controller*, Options);
    ~NodeAudioOutput();
    
    NodeOutput* getChannel(int);
    
    inline void apply() {};
    
};


#endif
