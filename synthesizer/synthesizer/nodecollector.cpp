#include "nodecollector.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"

NodeCollector::NodeCollector(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "collector";
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("input", "0.0")));
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    input->autoUpdate = false;
}
