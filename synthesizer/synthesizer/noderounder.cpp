#include "noderounder.hpp"
#include <cmath>

NodeRounder::NodeRounder(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "rounder";
    
    // Set options
    std::string f = options.getString("function", "round");
    if(f.compare("round") == 0) function = Round; else
    if(f.compare("floor") == 0) function = Floor; else
    if(f.compare("ceil") == 0) function = Ceil;
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeRounder::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    switch(function) {
        case Round:
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = std::round(input[x]);
            break;
        case Floor:
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = std::round(input[x]);
            break;
        case Ceil:
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = std::round(input[x]);
            break;
    }
}
