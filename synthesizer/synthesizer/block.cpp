#include "block.hpp"
#include "controller.hpp"
#include "units.hpp"
#include "unit.hpp"

Block::Block(Controller* controller) {
    // Store controller
    this->controller = controller;
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

Parameter* Block::getInput(std::string label) {
    mutexInputs.lock();
    Parameter* parameter = NULL;
    auto position = inputs.find(label);
    if(position != inputs.end())
        parameter = position->second;
    mutexInputs.unlock();
    return parameter;
}

Unit* Block::getOutput(std::string label) {
    mutexOutputs.lock();
    Unit* unit = NULL;
    auto position = outputs.find(label);
    if(position != outputs.end())
        unit = position->second;
    mutexOutputs.unlock();
    return unit;
}
