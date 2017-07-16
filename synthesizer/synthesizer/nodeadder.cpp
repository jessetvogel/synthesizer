#include "nodeadder.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "arguments.hpp"

const int NodeAdder::maxN = 16;

NodeAdder::NodeAdder(Controller* controller, Arguments arguments) : Node(controller) {
    // Set arguments
    n = arguments.getInteger("n", 1);
    keyNode = arguments.getBool("key", false);
    
    // Set type
    type = "adder";
    
    // Set inputs and outputs
    inputs = new NodeInput*[n];
    gains = new NodeInput*[n];
    for(int i = 0;i < n; i ++) {
        char strInput[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strGain[12];
        sprintf(strInput, "input_%d", i+1);
        sprintf(strGain, "gain_%d", i+1);
        addInput(strInput, inputs[i] = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
        addInput(strGain, gains[i] = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    }
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

NodeAdder::~NodeAdder() {
    delete[] inputs;
    delete[] gains;
}

void NodeAdder::apply() {
    float* output = this->output->getBuffer();
    memset(output, 0, sizeof(float) * framesPerBuffer);
    for(int i = 0;i < n; i++) {
        float* input = ((NodeOutput*) inputs[i]->pointer)->getBuffer();
        float* gain = ((NodeOutput*) gains[i]->pointer)->getBuffer();

        for(int x = 0;x < framesPerBuffer; ++x)
            output[x] += input[x] * gain[x];
    }
}
