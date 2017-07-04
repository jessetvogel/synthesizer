#include "blocks.hpp"
#include "block.hpp"
#include "controller.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "parameter.hpp"

#include "error.hpp"

Blocks::Blocks(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
}

Blocks::~Blocks() {
    mutex.lock();
    for(auto it = blocks.begin(); it != blocks.end(); ++it)
        delete *it;
    mutex.unlock();
}

bool Blocks::create(std::string id) {
    Block* block = getBlock(id);
    if(block != NULL) return false; // TODO
    
    block = new Block(controller);
    block->setId(id);
    mutex.lock();
    blocks.push_back(block);
    mutex.unlock();
    return true;
}

bool Blocks::destroy(std::string id) {
    mutex.lock();
    bool found = false;
    for(auto it = blocks.begin(); it != blocks.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            blocks.erase(it);
            delete *it;
            found = true;
            break;
        }
    }
    mutex.unlock();
    return found;
}

bool Blocks::addInput(std::string id, std::string label, std::string unitId, std::string parameterLabel) {
    Block* block = getBlock(id);
    if(block == NULL) return false; // TODO
    
    Unit* unit = controller->getUnits()->getUnit(unitId);
    if(unit == NULL) return false; // TODO
    
    Parameter* parameter = unit->getParameter(parameterLabel);
    if(parameter == NULL) return false; // TODO
    
    return block->addInput(label, parameter);
}

bool Blocks::addOutput(std::string id, std::string label, std::string unitId) {
    Block* block = getBlock(id);
    if(block == NULL) return false; // TODO
    
    Unit* unit = controller->getUnits()->getUnit(unitId);
    if(unit == NULL) return false; // TODO
    
    return block->addOutput(label, unit);
}

bool Blocks::set(std::string id, std::string label, std::string value) {
    Block* block = getBlock(id);
    if(block == NULL) return false; // TODO
    
    Parameter* parameter = block->getInput(label);
    if(parameter == NULL) return false; // TODO
    
    return parameter->set(value);
}

Block* Blocks::getBlock(std::string id) {
    mutex.lock();
    Block* block = NULL;
    for(auto it = blocks.begin(); it != blocks.end(); ++it) {
        if((*it)->getId().compare(id) == 0) {
            block = *it;
            break;
        }
    }
    mutex.unlock();
    return block;
}
