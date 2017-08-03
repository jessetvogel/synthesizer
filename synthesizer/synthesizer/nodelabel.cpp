#include "nodelabel.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeLabel::NodeLabel(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "label";
    
    // Key dependence
    voiceDependent = options.getBool("voice", false);
    
    // Set inputs and outputs
    addInput("value", value = new NodeInput(controller, voiceDependent ? NodeInput::NODE_VOICE : NodeInput::NODE, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeLabel::apply() {
    float* value = this->value->pointer->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = value[x];
    }
}
