#include <cmath>

#include "nodevolumemeter.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "arguments.hpp"

NodeVolumeMeter::NodeVolumeMeter(Controller* controller, Arguments arguments) : Node(controller) {
    // Set type
    type = "volumemeter";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    RMSAverage = 0.0;
    
    // Set inputs and outputs
    addInput("output", input = new NodeInput(controller, keyDependent ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeVolumeMeter::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        RMSAverage = 0.998f * RMSAverage + 0.002f * (input[x]*input[x]); // TODO: coefficients? (see the Audio Effects book, chapter 9,10 or 11 somewhere. Also, it should be called a envelope follower)
        output[x] = std::sqrt(RMSAverage);
    }
}
