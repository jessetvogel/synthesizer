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

NodeOscillator::NodeOscillator(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "oscillator";
    
    // Set options
    keyNode = options.getBool("key", false);
    
    // Set inputs and outputs
    addInput("sample", sample = new NodeInput(controller, NodeInput::SAMPLE, "sine"));
    addInput("frequency", frequency = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("amplitude", amplitude = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    addInput("mean", mean = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    // Create arrays
    if(keyNode) {
        phase = new double[SETTINGS_POLYPHONY];
        memset(phase, 0, sizeof(double) * SETTINGS_POLYPHONY);
    }
    else {
        phase = new double[1];
        phase[0] = 0.0;
    }
}

NodeOscillator::~NodeOscillator() {
    delete[] phase;
}

void NodeOscillator::apply() {
    Sample* sample = (Sample*) (this->sample->pointer);
    float* frequency = ((NodeOutput*) this->frequency->pointer)->getBuffer();
    float* amplitude = ((NodeOutput*) this->amplitude->pointer)->getBuffer();
    float* mean = ((NodeOutput*) this->mean->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    double t = 1.0 / sampleRate;
    int i = keyNode ? controller->getNodes()->currentKey->id : 0;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = mean[x] + amplitude[x] * sample->getValue(phase[i]);
        phase[i] += t * frequency[x] * 2.0 * M_PI;
    }
}
