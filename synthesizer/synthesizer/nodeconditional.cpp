#include "nodeconditional.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeConditional::NodeConditional(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "conditional";
    
    // Set options
    voiceDependent = options.getBool("voice", false);
    
    // Set inputs and outputs
    NodeInput::Type ___ = voiceDependent ? NodeInput::NODE_VOICE : NodeInput::NODE;
    addInput("input", input = new NodeInput(controller, ___, "0.0"));
    addInput("low", low = new NodeInput(controller, ___, "0.0"));
    addInput("high", high = new NodeInput(controller, ___, "0.0"));
    addInput("output_low", outputLow = new NodeInput(controller, ___, "0.0"));
    addInput("output_middle", outputMiddle = new NodeInput(controller, ___, "0.0"));
    addInput("output_high", outputHigh = new NodeInput(controller, ___, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeConditional::apply() {
    float* input = this->input->pointer->getBuffer();
    float* low = this->low->pointer->getBuffer();
    float* high = this->high->pointer->getBuffer();
    float* outputLow = this->outputLow->pointer->getBuffer();
    float* outputMiddle = this->outputMiddle->pointer->getBuffer();
    float* outputHigh = this->outputHigh->pointer->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        float y = input[x];
        if(y < low[x]) {
            output[x] = outputLow[x];
            continue;
        }
        if(y > high[x]) {
            output[x] = outputHigh[x];
            continue;
        }
        output[x] = outputMiddle[x];
    }
}
