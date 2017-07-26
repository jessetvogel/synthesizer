#include "nodelabel.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeLabel::NodeLabel(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "label";
    
    // Key dependence
    keyNode = options.getBool("key", false);
    
    // Set inputs and outputs
    addInput("value", value = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeLabel::apply() {
    float* value = ((NodeOutput*) this->value->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = value[x];
    }
}
