#include "nodecollector.hpp"
#include "controller.hpp"
#include "midistate.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeCollector::NodeCollector(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "collector";
    
    // Should not update parameters (as this is done by controller->nodes)
    updateNodeInputs = false;
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, NodeInput::NODE, "0.0"));
    addInput("release_time", releaseTime = new NodeInput(controller, NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeCollector::addKeyEvent(KeyEvent* keyEvent) {
    keyEvents.push_back(keyEvent);
}

double NodeCollector::getReleaseTime() {
    return ((NodeOutput*) releaseTime->pointer)->getBuffer()[0];
}
