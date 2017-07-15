#ifndef nodefactory_hpp
#define nodefactory_hpp

#include <string>

class Controller;
class Nodes;
class Node;
class Arguments;

class NodeFactory {
    
public:
    
    static void createDefaultNodes(Controller*, Nodes*);
    static Node* create(Controller*, std::string, std::string, Arguments);
    
};

#endif
