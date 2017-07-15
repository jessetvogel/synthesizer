#include "nodelabel.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "arguments.hpp"

NodeLabel::NodeLabel(Controller* controller, Arguments arguments) : Node(controller) {
    // Set type
    type = "label";
    
    // Key dependence
    keyDependent = arguments.getBool("key", false);
    
    // Set inputs and outputs
    addInput("value", value = new NodeInput(controller, keyDependent ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeLabel::apply() {
    float* value = ((NodeOutput*) this->value->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = value[x];
    }
}
