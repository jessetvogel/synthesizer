#ifndef node_hpp
#define node_hpp

#include <unordered_map>
#include <string>

#include "options.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"

#define NODE_DEFAULT_ID "#undefined"
#define NODE_HIDDEN_ID "#hidden"
#define NODE_DEFAULT_TYPE "#undefined"

#define NODE_OUTPUT_DEFAULT "default"

class Controller;
class Options;

class Node {
    
private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    
    std::string id = NODE_DEFAULT_ID;
    std::string type = NODE_DEFAULT_TYPE;
    
    std::unordered_map<std::string, NodeInput*> inputs;
    std::unordered_map<std::string, NodeOutput*> outputs;
    
    bool voiceDependent;
    bool hidden;
    bool active;
    
    unsigned long framesPerBuffer;
    double sampleRate;
    
    inline virtual void apply() {};
    
public:
    
    Node(Controller*);
    ~Node();
    
    Node* setId(std::string);
    inline std::string getId() { return id; }
    inline std::string getType() { return type; }
    
    bool addInput(std::string, NodeInput*);
    bool addOutput(std::string, NodeOutput*);
    
    bool removeInput(NodeInput*);
    bool removeOutput(NodeOutput*);
    
    NodeInput* getInput(std::string);
    NodeOutput* getOutput(std::string);
    
    inline void reset() { updated = false; };
    void update();
    
    inline bool isVoiceDependent() { return voiceDependent; };
    inline void hide() { hidden = true; id = NODE_HIDDEN_ID; }
    inline void setActive(bool active) { this->active = active; }
    inline bool isActive(bool active) { return active; }
    inline bool isHidden() { return hidden; };
  
    bool dependsOn(Node*);
    
    // Status
    void printNode();
    
};

#endif
