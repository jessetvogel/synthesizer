#include "nodefuzz.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeFuzz::NodeFuzz(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "fuzz";
    
    // Set options
    keyNode = options.getBool("key", false);
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("input_gain", inputGain = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeFuzz::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    float* inputGain = ((NodeOutput*) this->inputGain->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = fuzz(input[x], inputGain[x]);
}

double NodeFuzz::fuzz(double input, double inputGain) {
    return ((input > 0.0) ? 1.0 : ((input < 0.0) ? -1.0 : 0.0)) * (1.0 - std::exp(- inputGain * std::abs(input))) / (1.0 - std::exp(- inputGain));
}
