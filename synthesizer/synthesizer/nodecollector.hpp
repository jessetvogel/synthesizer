#ifndef nodecollector_hpp
#define nodecollector_hpp

#include "node.hpp"
#include "keyevent.hpp"

class NodeCollector : public Node {
    
    NodeInput* input;
    NodeInput* releaseTime;
    
    NodeOutput* output;
    
    std::vector<KeyEvent*> keyEvents;
    
public:
    
    NodeCollector(Controller*, Arguments);
    
    void addKeyEvent(KeyEvent*);
    inline std::vector<KeyEvent*>* getKeyEvents() { return &keyEvents; }
    
    double getReleaseTime();
    
    inline NodeOutput* getOutput() { return output; }
    inline NodeInput* getInput() { return input; }
    
    inline void apply() {}
    
};

#endif
