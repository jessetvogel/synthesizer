#include <cmath>

#include "nodePWM.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "settings.hpp"
#include "options.hpp"
#include "voice.hpp"

NodePWM::NodePWM(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "PWM";
    
    // Set options
    voiceDependent = options.getBool("voice", false);
    
    // Set inputs and outputs
    NodeInput::Type ___ = voiceDependent ? NodeInput::NODE_VOICE : NodeInput::NODE;
    addInput("frequency", frequency = new NodeInput(controller, ___, options.getString("frequency", "0.0")));
    addInput("duty", duty = new NodeInput(controller, ___, options.getString("duty", "0.5")));
    addInput("low", low = new NodeInput(controller, ___, options.getString("low", "0.0")));
    addInput("high", high = new NodeInput(controller, ___, options.getString("high", "1.0")));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    // Create arrays
    int voices = voiceDependent ? controller->getSettings()->voices : 1;
    phase = new double[voices];
    memset(phase, 0, sizeof(double) * voices);
}

NodePWM::~NodePWM() {
    delete[] phase;
}

void NodePWM::apply() {
    float* frequency = this->frequency->pointer->getBuffer();
    float* duty = this->duty->pointer->getBuffer();
    float* low = this->low->pointer->getBuffer();
    float* high =  this->high->pointer->getBuffer();
   
    float* output = this->output->getBuffer();
    
    double t = 1.0 / sampleRate;
    int i = voiceDependent ? controller->getNodes()->currentVoice->id : 0;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = phase[i] < duty[x] ? high[x] : low[x];
        phase[i] += t * frequency[x];
        if(phase[i] >= 1.0) phase[i] -= floor(phase[i]);
    }
}
