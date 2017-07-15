#ifndef nodeoutput_hpp
#define nodeoutput_hpp

#include <string>

class Controller;
class Node;

class NodeOutput {

    Controller* controller;

    Node* node;
    float* buffer;
    
public:
    
    NodeOutput(Controller*, Node*);
    ~NodeOutput();
    
    inline Node* getNode() { return node; }
    inline float* getBuffer() { return buffer; }
    
};

#endif
