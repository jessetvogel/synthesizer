#include "block.hpp"
#include "unit.hpp"

Block::Block(Controller* controller) {
    // Store controller
    this->controller = controller;
}

Block::~Block() {
    for(auto it = attachments.begin();it != attachments.end();++ it)
        delete *it;
}

bool Block::addInput(std::string label, Parameter* parameter) {
    if(inputs.find(label) != inputs.end()) {
        // TODO: error: label already used
        return false;
    }
    
    inputs[label] = parameter;
    return true;
}

bool Block::addOutput(std::string label, Unit* unit) {
    if(outputs.find(label) != outputs.end()) {
        // TODO: error: label already used
        return false;
    }
    
    outputs[label] = unit;
    return true;
}

bool Block::attachUnit(Unit* unit) {
    attachments.push_back(unit);
    return true;
}
