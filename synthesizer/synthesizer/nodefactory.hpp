#ifndef nodefactory_hpp
#define nodefactory_hpp

#include <string>

class Controller;
class Nodes;
class Node;
class Options;

class NodeFactory {
    
public:
    
    static void createDefaultNodes(Controller*, Nodes* nodes); // We do need to pass Nodes* as it is called in the constructor of Nodes
    static Node* create(Controller*, std::string, std::string, Options);
    
};

#endif
