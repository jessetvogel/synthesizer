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
}

bool NodeInput::set(std::string value) {
    switch(type) {

        case SAMPLE:
            if(value.compare("sine") == 0)      { pointer = Sample::sine; expression = value; return true; }
            if(value.compare("square") == 0)    { pointer = Sample::square; expression = value; return true; }
            if(value.compare("triangle") == 0)  { pointer = Sample::triangle; expression = value; return true; }
            if(value.compare("sawtooth") == 0)  { pointer = Sample::sawtooth; expression = value; return true; }
            Status::addError("Provided sample does not exist");
            return false;
            
        case CURVE:
            if(value.compare("linear") == 0)        { pointer = Curve::Linear; expression = value; return true; }
            if(value.compare("sine") == 0)          { pointer = Curve::Sine; expression = value; return true; }
            if(value.compare("quadin") == 0)        { pointer = Curve::QuadIn; expression = value; return true; }
            if(value.compare("quadout") == 0)       { pointer = Curve::QuadOut; expression = value; return true; }
            if(value.compare("quartin") == 0)       { pointer = Curve::QuartIn; expression = value; return true; }
            if(value.compare("quartout") == 0)      { pointer = Curve::QuartOut; expression = value; return true; }
            if(value.compare("exponential") == 0)   { pointer = Curve::Exponential; expression = value; return true; }
            Status::addError("Provided curve does not exist");
            return false;
            
        case NODE:
        case NODE_KEY_INDEPENDENT:
            
            NodeOutput* nodeOutput = NULL;
            // Check if it is a number
            if(Util::isNumber(value)) {
                // Create a new constant
                nodeOutput = controller->getNodes()->createConstant(stod(value));
            }
            
            // Check if it is a node output
            else if((nodeOutput = controller->getNodes()->getNodeOutput(value)) != NULL) {
                // Check for key dependence
                if(type == NODE_KEY_INDEPENDENT && nodeOutput->getNode()->isKeyDependent()) {
                    Status::addError("Node cannot depend on key node");
                    return false;
                }
            }
            
            if(nodeOutput == NULL) { Status::addError("Provided node output does not exist"); return false; }
            
            // If we are overwriting a node output which has NULL node, delete it
            if(pointer != NULL) {
                Node* node = ((NodeOutput*) pointer)->getNode();
                if(node == NULL)
                    delete node;
            }
            
            pointer = nodeOutput;
            expression = value;
            return true;
    }
}

std::string NodeInput::typeToString(Type type) {
    switch(type) {
        case NODE: return "key node";
        case NODE_KEY_INDEPENDENT: return "node";
        case SAMPLE: return "sample";
        case CURVE: return "curve";
    }
}
