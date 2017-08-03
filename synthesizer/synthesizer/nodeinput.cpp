#include <string>

#include "nodeinput.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "node.hpp"
#include "sample.hpp"
#include "util.hpp"
#include "curve.hpp"

#include "status.hpp"

NodeInput::NodeInput(Controller* controller, Type type, std::string defaultValue) : type(type) {
    // Store stuff and set default value
    this->controller = controller;
    
    pointer = NULL;
    set(defaultValue);
    expression = defaultValue;
    autoUpdate = true;
}

bool NodeInput::set(std::string value) {
    NodeOutput* nodeOutput = NULL;
    
    // Check if it is a number
    if(Util::isNumber(value)) {
        // Create a new constant
        nodeOutput = controller->getNodes()->createConstant(stod(value));
    }
    
    // Check if it is some node's output
    else if((nodeOutput = controller->getNodes()->getNodeOutput(value)) != NULL) {
        // Check for key dependence
        if(type == NODE && nodeOutput->getNode()->isVoiceDependent()) {
            Status::addError("Node cannot depend on voice dependent node");
            return false;
        }
    }
    
    // Check for keywords
    else {
        double x = -1.0;
        if(value.compare("sine") == 0)          x = SAMPLE_SINE; else
        if(value.compare("triangle") == 0)      x = SAMPLE_TRIANGLE; else
        if(value.compare("sawtooth") == 0)      x = SAMPLE_SAWTOOTH; else
        if(value.compare("square") == 0)        x = SAMPLE_SQUARE; else
            
            
        if(value.compare("linear") == 0)        x = CURVE_LINEAR; else
        if(value.compare("sine") == 0)          x = CURVE_SINE; else
        if(value.compare("quadin") == 0)        x = CURVE_QUADIN; else
        if(value.compare("quadout") == 0)       x = CURVE_QUADOUT; else
        if(value.compare("quartin") == 0)       x = CURVE_QUARTIN; else
        if(value.compare("quartout") == 0)      x = CURVE_QUARTOUT; else
        if(value.compare("exponential") == 0)   x = CURVE_EXPONENTIAL;
        
        if(x != -1.0)
            nodeOutput = controller->getNodes()->createConstant(x);
    }
    
    if(nodeOutput == NULL) {
        Status::addError("Provided node output does not exist");
        return false;
    }
    
    // If we are overwriting a node output which has NULL node, delete it
    if(pointer != NULL) {
        Node* node = pointer->getNode();
        if(node == NULL)
            delete node;
    }
    
    pointer = nodeOutput;
    expression = value;
    return true;
}

std::string NodeInput::typeToString(Type type) {
    switch(type) {
        case NODE: return "node";
        case NODE_VOICE: return "voice node";
        case SAMPLE: return "sample";
        case CURVE: return "curve";
    }
}
