#include "nodeglider.hpp"
#include <cmath>

NodeGlider::NodeGlider(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "glider";
    
    // Default values
    value = 1.0;
    DT = 0.0;
    
    // Set inputs and outputs
    addInput("destination", destination = new NodeInput(controller, NodeInput::NODE, "1.0"));
    addInput("doubling_time", doublingTime = new NodeInput(controller, NodeInput::NODE, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeGlider::apply() {
    float* destination = this->destination->pointer->getBuffer();
    float* doublingTime = this->doublingTime->pointer->getBuffer();

    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        if(DT <= 0.0) // TODO: 0.0?
            value = destination[x];
        else {
            if(value < destination[x]) {
                value *= std::pow(2.0, 1.0 / (DT * sampleRate));
                if(value > destination[x]) value = destination[x];
            }
            else if(value > destination[x]) {
                value *= std::pow(2.0, -1.0 / (DT * sampleRate));
                if(value < destination[x]) value = destination[x];
            }
        }
        output[x] = value;
        DT = doublingTime[x];
    }
}
