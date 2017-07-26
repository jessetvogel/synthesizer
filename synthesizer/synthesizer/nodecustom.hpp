#ifndef nodecustom_hpp
#define nodecustom_hpp

#include <vector>

#include "node.hpp"

class NodeCustom : public Node {
    
    std::vector<Node*> attachments;
    
public:
    
    NodeCustom(Controller*, Options);
    ~NodeCustom();
    
    bool attach(Node* node);
    
    inline void apply() {}
    
};


#endif
