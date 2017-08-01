#include <cmath>

#include "nodefollower.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeFollower::NodeFollower(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "follower";
    
    // Set options
    keyNode = options.getBool("key", false);
    value = 0.0;
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("attack_time", attackTime = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.01")); // TODO
    addInput("decay_time", decayTime = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.01")); // TODO
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeFollower::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    float* attackTime = ((NodeOutput*) this->attackTime->pointer)->getBuffer();
    float* decayTime = ((NodeOutput*) this->decayTime->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    double alpha;
    for(int x = 0;x < framesPerBuffer; ++x) {
        double q = std::abs(input[x]);
        if(q > value) {
            if(attackTime[x] <= 0.0)
                alpha = 0.0;
            else
                alpha = std::exp(-1.0 / (attackTime[x] * sampleRate));
        }
        else {
            if(decayTime[x] <= 0.0)
                alpha = 0.0;
            else
                alpha = std::exp(-1.0 / (decayTime[x] * sampleRate));
        }
        
        value = alpha * value + (1.0 - value) * q;
        output[x] = value;
    }
}
