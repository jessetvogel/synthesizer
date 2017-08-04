#ifndef nodes_hpp
#define nodes_hpp

#include <vector>
#include <unordered_map>
#include <mutex>
#include "keystate.hpp"

class Controller;
class Node;
class NodeCollector;
class NodeAudioOutput;
class NodeParameter;
class NodeInput;
class NodeOutput;
class Voice;

class Nodes {
    
    Controller* controller;
    
    std::vector<Node*> nodes;
    std::vector<NodeCollector*> collectors;
    std::vector<NodeAudioOutput*> audioOutputs;
    std::vector<NodeParameter*> parameters;
    
    std::vector<NodeOutput*> constants;
    
    std::mutex mutex;
    
public:
    
    Nodes(Controller*);
    ~Nodes();
    
    bool addNode(Node*);
    Node* getNode(std::string);
    
    NodeOutput* createConstant(double);
    bool deleteConstant(NodeOutput*);
    
    bool addCollector(NodeCollector*);
    bool removeCollector(NodeCollector*);
    
    bool addAudioOutput(NodeAudioOutput*);
    bool removeAudioOutput(NodeAudioOutput*);
    
    bool addNodeParameter(NodeParameter*);
    bool updateNodeParameter(int, double);
    bool removeNodeParameter(NodeParameter*);
    
    Voice* currentVoice;
    void addKeyEvent(KeyEvent*);
    
    void resetNodes();
    void resetNodesVoiceDependent();
    
    bool apply();
    
    NodeInput* getNodeInput(std::string);
    NodeOutput* getNodeOutput(std::string);
    
    bool clear();
    
    // Commands
    bool create(std::string, std::string, std::string);
    bool destroy(std::string);
    bool rename(std::string, std::string);
    bool set(std::string, std::string);
    bool hide(std::string);
    bool info(std::string);
    bool activate(std::string);
    bool deactivate(std::string);
    
    bool addInput(std::string, std::string, std::string);
    bool addOutput(std::string, std::string, std::string);
    bool attach(std::string, std::string);
    
    // Status
    void printNodes();
    
};

#endif
