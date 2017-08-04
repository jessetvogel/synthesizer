#include "nodeparameter.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "curve.hpp"
#include "util.hpp"
#include "options.hpp"

#include "log.hpp"

NodeParameter::NodeParameter(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "parameter";
    
    // Set options
    midiCC = options.getInteger("midi_cc", -1);
    value = options.getDouble("value", 0.0);
    
    double _min = options.getDouble("min" ,0.0);
    double _max = options.getDouble("max" ,1.0);
    
    // Set inputs and outputs
    addInput("midi_cc", cc = new NodeInput(controller, NodeInput::NODE, std::to_string(midiCC)));
    addInput("min", min = new NodeInput(controller, NodeInput::NODE, std::to_string(_min)));
    addInput("max", max = new NodeInput(controller, NodeInput::NODE, std::to_string(_max)));
    addInput("curve", curve = new NodeInput(controller, NodeInput::CURVE, "linear"));
    addInput("value", valueInput = new NodeInput(controller, NodeInput::NODE, "-1.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeParameter::apply() {
    float* cc = this->cc->pointer->getBuffer();
    float* min = this->min->pointer->getBuffer();
    float* max = this->max->pointer->getBuffer();
    Curve* curve = Curve::get(this->curve->pointer->getBuffer()[0]);
    float* valueInput = this->valueInput->pointer->getBuffer();
    
    float* output = this->output->getBuffer();
    
    // In case value is manually set, update it and set valueInput again to -1.0
    if(valueInput[0] >= 0.0 && valueInput[0] <= 1.0) {
        setValue(valueInput[0]);
        valueInput[0] = -1.0; // TODO please solve this in a better way
    }
    
    // Determine midiCC
    midiCC = (int) cc[0];
    
    // Linearly interpolate between min and max
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Curve::ease(min[x], max[x], value, curve);
}

void NodeParameter::setValue(double value) {
    this->value = value;
}
