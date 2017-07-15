#include "nodeoutput.hpp"
#include "controller.hpp"
#include "settings.hpp"

NodeOutput::NodeOutput(Controller* controller, Node* node) {
    // Store everything!
    this->controller = controller;
    this->node = node;
    
    // Create buffer
    buffer = new float[controller->getSettings()->bufferSize];
}

NodeOutput::~NodeOutput() {
    delete[] buffer;
}
