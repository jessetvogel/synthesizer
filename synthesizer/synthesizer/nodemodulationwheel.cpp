#include "nodemodulationwheel.hpp"
#include "controller.hpp"
#include "midistate.hpp"
#include "nodeoutput.hpp"

NodeModulationWheel::NodeModulationWheel(Controller* controller) : Node(controller) {
    // Set type
    type = "modulation_wheel";
    
    // Hidden
    hidden = true;
    
    // Set outputs
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeModulationWheel::apply() {
    float* output = this->output->getBuffer();
    
    double value = controller->getMidiState()->modulationWheel;
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = value;
}
