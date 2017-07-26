#include "nodeparameter.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "curve.hpp"
#include "util.hpp"
#include "options.hpp"

NodeParameter::NodeParameter(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "parameter";
    
    // Set options
    midiCC = options.getInteger("midiCC", -1);
    value = options.getDouble("initial", 0.0);
    
    // Hidden
    hidden = true;
    
    // Set inputs and outputs
    addInput("min", min = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("max", max = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    addInput("curve", curve = new NodeInput(controller, NodeInput::CURVE, "linear"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeParameter::apply() {
    float* min = ((NodeOutput*) this->min->pointer)->getBuffer();
    float* max = ((NodeOutput*) this->max->pointer)->getBuffer();
    Curve* curve = (Curve*) (this->curve->pointer);
    
    float* output = this->output->getBuffer();
    
    // Linearly interpolate between min and max
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Curve::ease(min[x], max[x], value, curve);
}

void NodeParameter::setValue(double value) {
    this->value = value;
}
