#ifndef nodecollector_hpp
#define nodecollector_hpp

#include "node.hpp"
#include "keystate.hpp"
#include "voice.hpp"

class NodeCollector : public Node {
    
protected:
    
    NodeInput* input;
    NodeOutput* output;
    
public:
    
    NodeCollector(Controller*, Options);
    virtual inline ~NodeCollector() {};
    
    virtual inline void addKeyEvent(KeyEvent*) {};
    virtual inline void apply() {}
    
};

#endif
