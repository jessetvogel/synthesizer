#include "monitor.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "nodeoutput.hpp"
#include "node.hpp"
#include "status.hpp"

Monitor::Monitor(Controller* controller) {
    // Set controller
    this->controller = controller;
}

bool Monitor::monitor(std::string nodeOutputLabel) {
    nodeOutput = controller->getNodes()->getNodeOutput(nodeOutputLabel);
    
    if(nodeOutput == NULL) { Status::addError("Provided node output not found"); return false; }
    if(nodeOutput->getNode()->isKeyDependent()) { Status::addError("Cannot monitor key dependent node output"); return false; }
    
    Status::addExtra("monitor");
    return true;
}
