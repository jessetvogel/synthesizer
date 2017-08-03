#include "nodecollector.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeCollector::NodeCollector(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "collector";
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, NodeInput::NODE_VOICE, "0.0"));
    input->autoUpdate = false;
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}
