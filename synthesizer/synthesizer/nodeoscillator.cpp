#include <cmath>

#include "nodeoscillator.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "sample.hpp"
#include "settings.hpp"
#include "options.hpp"
#include "voice.hpp"

NodeOscillator::NodeOscillator(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "oscillator";
    
    // Set options
    voiceDependent = options.getBool("voice", false);
    
    // Set inputs and outputs
    NodeInput::Type ___ = voiceDependent ? NodeInput::NODE_VOICE : NodeInput::NODE;
    addInput("sample", sample = new NodeInput(controller, NodeInput::SAMPLE, "sine"));
    addInput("frequency", frequency = new NodeInput(controller, ___, "0.0"));
    addInput("amplitude", amplitude = new NodeInput(controller, ___, "1.0"));
    addInput("mean", mean = new NodeInput(controller, ___, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    // Create arrays
    int voices = voiceDependent ? controller->getSettings()->voices : 1;
    phase = new double[voices];
    memset(phase, 0, sizeof(double) * voices);
}

NodeOscillator::~NodeOscillator() {
    delete[] phase;
}

void NodeOscillator::apply() {
    Sample* sample = Sample::get(this->sample->pointer->getBuffer()[0]);
    float* frequency = this->frequency->pointer->getBuffer();
    float* amplitude = this->amplitude->pointer->getBuffer();
    float* mean = this->mean->pointer->getBuffer();
    
    float* output = this->output->getBuffer();
    
    double t = 1.0 / sampleRate;
    int i = voiceDependent ? controller->getNodes()->currentVoice->id : 0;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = mean[x] + amplitude[x] * sample->getValue(phase[i]);
        phase[i] += t * frequency[x] * M_2PI;
        while(phase[i] > M_2PI) phase[i] -= M_2PI;
    }
}
