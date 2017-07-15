#include <cmath>

#include "nodePWM.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "settings.hpp"
#include "arguments.hpp"

NodePWM::NodePWM(Controller* controller, Arguments arguments) : Node(controller) {
    // Set type
    type = "PWM";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set inputs and outputs
    addInput("frequency", frequency = new NodeInput(controller, keyDependent ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("duty", duty = new NodeInput(controller, keyDependent ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("low", low = new NodeInput(controller, keyDependent ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("high", high = new NodeInput(controller, keyDependent ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    // Create arrays
    if(keyDependent) {
        phase = new double[SETTINGS_POLYPHONY];
        memset(phase, 0, sizeof(double) * SETTINGS_POLYPHONY);
    }
    else {
        phase = new double[1];
        phase[0] = 0.0;
    }
}

NodePWM::~NodePWM() {
    delete[] phase;
}

void NodePWM::apply() {
    float* frequency = ((NodeOutput*) this->frequency->pointer)->getBuffer();
    float* duty = ((NodeOutput*) this->duty->pointer)->getBuffer();
    float* low = ((NodeOutput*) this->low->pointer)->getBuffer();
    float* high = ((NodeOutput*) this->high->pointer)->getBuffer();
   
    float* output = this->output->getBuffer();
    
    double t = 1.0 / sampleRate;
    int i = keyDependent ? controller->getNodes()->currentKey->id : 0;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = phase[i] < duty[x] ? high[x] : low[x];
        phase[i] += t * frequency[x];
        if(phase[i] >= 1.0) phase[i] -= floor(phase[i]);
    }
}
