#include <algorithm>

#include "unit.hpp"
#include "controller.hpp"
#include "units.hpp"
#include "unitconstant.hpp"
#include "unitparameter.hpp"

#include "unitfactory.hpp"
#include "arguments.hpp"

Units::Units(Controller* controller) {
    // Store controller
    this->controller = controller;
    
    // Default units
    UnitFactory::createDefaultUnits(controller, this);
}

Units::~Units() {
    mutexUnits.lock();
    for(auto it = units.begin(); it != units.end(); ++it)
        delete *it;

    for(auto it = constants.begin(); it != constants.end(); ++it)
        delete *it;
    // Note that the parameters are a subset of units, hence they need not be deallocated (in fact, it will result in errors)
    mutexUnits.unlock();
}

bool Units::create(std::string type, std::string id, std::string arguments) {
    Unit* unit = get(id);
    if(unit != NULL) return false; // TODO: (search for 'return false')
    
    Arguments args(controller, arguments);
    unit = UnitFactory::create(controller, type, id, args);
    if(unit == NULL) return false;
    
    mutexUnits.lock();
    units.push_back(unit);
    mutexUnits.unlock();
    return true;
}

bool Units::remove(std::string id) {
    mutexUnits.lock();
    bool found = false;
    for(auto it = units.begin(); it != units.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            units.erase(it);
            delete *it;
            found = true;
            break;
        }
    }
    mutexUnits.unlock();
    return found;
}

bool Units::rename(std::string oldId, std::string newId) {
    Unit* unit = get(oldId);
    if(unit == NULL) return false; // TODO
    
    Unit* unitNew = get(newId);
    if(unitNew != NULL) return false;
    
    return unit->setId(newId);
}

bool Units::setValue(std::string id, std::string parameter, std::string value) {
    Unit* unit = get(id);
    if(unit == NULL) return false; // TODO
    
    return unit->setParameter(parameter, value);
}

bool Units::add(Unit* unit) {
    mutexUnits.lock();
    units.push_back(unit);
    mutexUnits.unlock();
    return true;
}

bool Units::remove(Unit* unit) {
    return remove(unit->getId()); // TODO: yeahh... should be a better way to structure this
}

Unit* Units::get(std::string id) {
    mutexUnits.lock();
    Unit* unit = NULL;
    for(auto it = units.begin(); it != units.end(); ++it) {
        if((*it)->getId().compare(id) == 0) {
            unit = *it;
            break;
        }
    }
    mutexUnits.unlock();
    return unit;
}

UnitConstant* Units::createConstant(double value) {
    mutexConstants.lock();
    UnitConstant* constant = new UnitConstant(controller, value);
    constants.push_back(constant);
    mutexConstants.unlock();
    return constant;
}

bool Units::deleteConstant(UnitConstant* constant) {
    mutexConstants.lock();
    bool found = false;
    auto position = std::find(constants.begin(), constants.end(), constant);
    if(position != constants.end()) {
        constants.erase(position);
        delete constant;
        mutexConstants.unlock();
        found = true;
    }
    mutexConstants.unlock();
    return found;
}

bool Units::isConstant(Unit* unit) {
    mutexConstants.lock();
    bool found = std::find(constants.begin(), constants.end(), unit) != constants.end();
    mutexConstants.unlock();
    return found;
}

bool Units::addParameter(UnitParameter* parameter, int MIDICC) {
    mutexParameters.lock();
    bool success = false;
    if(parameters.find(MIDICC) == parameters.end()) {
        parameters[MIDICC] = parameter;
        success = true;
    }
    mutexParameters.unlock();
    return success;
}

UnitParameter* Units::getParameter(int MIDICC) {
    mutexParameters.lock();
    UnitParameter* parameter = NULL;
    auto position = parameters.find(MIDICC);
    if(position != parameters.end())
        parameter = position->second;
    mutexParameters.unlock();
    return parameter;
}

bool Units::deleteParameter(int MIDICC) {
    mutexParameters.lock();
    bool success = false;
    auto position = parameters.find(MIDICC);
    if(position != parameters.end()) {
        delete position->second;
        parameters.erase(position);
        success = true;
    }
    mutexParameters.unlock();
    return success;
}

void Units::resetUnits() {
    mutexUnits.lock();
    for(auto it = units.begin(); it != units.end(); ++it) {
        (*it)->reset();
    }
    mutexUnits.unlock();
}

void Units::resetUnitsKeyDependent() {
    mutexUnits.lock();
    for(auto it = units.begin(); it != units.end(); ++it) {
        Unit* unit = *it;
        if(unit->isKeyDependent())
            unit->reset();
    }
    mutexUnits.unlock();
}
