#include <cstdlib>

#include "nodes.hpp"
#include "node.hpp"
#include "util.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"

#include "status.hpp"

Node::Node(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store some variables
    framesPerBuffer = controller->getSettings()->bufferSize;
    sampleRate = controller->getSettings()->sampleRate;
    
    // Default values
    voiceDependent = false;
    hidden = false;
    active = true;
}

Node::~Node() {
    // Delete all inputs and outputs
    for(auto it = inputs.begin(); it != inputs.end(); ++it)
        delete it->second;

    for(auto it = outputs.begin(); it != outputs.end(); ++it)
        delete it->second;
}

Node* Node::setId(std::string id) {
    this->id = id;
    return this;
}

void Node::update() {
    // If already updated, don't do anything
    if(updated) return;
    updated = true;
    
    // If non-active, set all outputs to zero and stop
    if(!active) {
        for(auto it = outputs.begin(); it != outputs.end(); ++it)
            memset(it->second->getBuffer(), 0, sizeof(float) * framesPerBuffer); // TODO: this done correctly?
        return;
    }
    
    // Update all node inputs (except if specified not to)
    for(auto it = inputs.begin(); it != inputs.end(); ++it) {
        NodeInput* input = it->second;
        if(!input->autoUpdate) continue;
        
        Node* node = input->pointer->getNode();
        if(node != NULL)
            node->update();
    }
    
    apply();
};

bool Node::dependsOn(Node* node) {
    for(auto it = inputs.begin(); it != inputs.end(); ++it) {
        NodeInput* input = it->second;
        if(input->pointer->getNode() == node)
            return true;
    }
    return false;
}

bool Node::addInput(std::string label, NodeInput* input) {
    auto position = inputs.find(label);
    if(position != inputs.end()) { Status::addError("Label for input is already used"); return false; }
    
    inputs[label] = input;
    return true;
}

bool Node::addOutput(std::string label, NodeOutput* output) {
    auto position = outputs.find(label);
    if(position != outputs.end()) { Status::addError("Label for output is already used"); return false; }
    
    outputs[label] = output;
    return true;
}

bool Node::removeInput(NodeInput* input) {
    bool found = false;
    for(auto it = inputs.begin();it != inputs.end(); ++it) {
        if(it->second == input) {
            inputs.erase(it);
            found = true;
            break;
        }
    }
    return found;
}

bool Node::removeOutput(NodeOutput* output) {
    bool found = false;
    for(auto it = outputs.begin();it != outputs.end(); ++it) {
        if(it->second == output) {
            outputs.erase(it);
            found = true;
            break;
        }
    }
    return found;
}

NodeInput* Node::getInput(std::string label) {
    auto position = inputs.find(label);
    if(position == inputs.end()) return NULL;
    
    return position->second;
}

NodeOutput* Node::getOutput(std::string label) {
    auto position = outputs.find(label);
    if(position == outputs.end()) return NULL;
    
    return position->second;
}

