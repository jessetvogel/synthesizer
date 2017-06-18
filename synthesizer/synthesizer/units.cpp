#include <algorithm>

#include "unit.hpp"
#include "controller.hpp"
#include "units.hpp"
#include "unitconstant.hpp"
#include "unitparameter.hpp"

#include "unitkeyinfo.hpp"
#include "unitleadkeyinfo.hpp"
#include "unitkeyoutput.hpp"
#include "unitmodulationwheel.hpp"

Units::Units(Controller* controller) {
    // Store controller
    this->controller = controller;
    
    // Default units
    add((new UnitKeyInfo(controller, UnitKeyInfo::Frequency))->setId("key_frequency"));
    add((new UnitKeyInfo(controller, UnitKeyInfo::Velocity))->setId("key_velocity"));
    add((new UnitKeyInfo(controller, UnitKeyInfo::Duration))->setId("key_duration"));
    add((new UnitKeyInfo(controller, UnitKeyInfo::Release))->setId("key_release"));

    add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Frequency))->setId("lead_key_frequency"));
    add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Velocity))->setId("lead_key_velocity"));
    add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Duration))->setId("lead_key_duration"));
    add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Release))->setId("lead_key_release"));
    add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Pressing))->setId("lead_key_pressing"));

    add((new UnitKeyOutput(controller))->setId("key_output"));
    add((new UnitModulationWheel(controller))->setId("modulation_wheel"));
}

Units::~Units() {
    mutexUnits.lock();
    for(auto it = units.begin(); it != units.end(); ++it)
        delete *it;

    for(auto it = constants.begin(); it != constants.end(); ++it)
        delete *it;

    for(auto it = parameters.begin(); it != parameters.end(); ++it)
        delete it->second;
    mutexUnits.unlock();
}

bool Units::add(Unit* unit) {
    mutexUnits.lock();
    units.push_back(unit);
    mutexUnits.unlock();
    return true;
}

bool Units::remove(Unit* unit) {
    mutexUnits.lock();
    bool found = false;
    for(auto it = units.begin(); it != units.end(); ++it) {
        if((*it) == unit) {
            units.erase(it);
            delete unit;
            found = true;
            break;
        }
    }
    mutexUnits.unlock();
    return found;
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

void Units::resetUnitsKeyDependent() {
    mutexUnits.lock();
    for(auto it = units.begin(); it != units.end(); ++it) {
        Unit* unit = *it;
        if(unit->isKeyDependent())
            unit->reset();
    }
    mutexUnits.unlock();
}
