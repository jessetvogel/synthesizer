#include <algorithm>

#include "units.hpp"
#include "unit.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "settings.hpp"
#include "unitconstant.hpp"
#include "unitparameter.hpp"
#include "unitaudiooutput.hpp"
#include "unitcollector.hpp"
#include "midistate.hpp"

#include "unitfactory.hpp"
#include "arguments.hpp"

Units::Units(Controller* controller) {
    // Store controller
    this->controller = controller;
    
    // Default units
    UnitFactory::createDefaultUnits(controller, this);
}

Units::~Units() {
    mutex.lock();
    for(auto it = units.begin(); it != units.end(); ++it)
        delete *it;

    for(auto it = constants.begin(); it != constants.end(); ++it)
        delete *it;
    // Note that the parameters and collectors are a subset of units, hence they need not be deallocated (in fact, double deallocation will result in errors)
    mutex.unlock();
}

bool Units::create(std::string type, std::string id, std::string arguments) {
    // Check if unit with this id already exists
    Unit* unit = getUnit(id);
    if(unit != NULL) return false; // TODO: (search for 'return false')
    
    // Create new unit
    Arguments args(controller, arguments);
    unit = UnitFactory::create(controller, type, id, args);
    if(unit == NULL) return false;
    
    // Append unit to relevant lists
    addUnit(unit);
    std::string unitType = unit->getType();
    if(unitType.compare("parameter") == 0) addParameter((UnitParameter*) unit);
    if(unitType.compare("collector") == 0) addCollector((UnitCollector*) unit);
    if(unitType.compare("audio_output") == 0) addAudioOutput((UnitAudioOutput*) unit);
    return true;
}

bool Units::destroy(std::string id) {
    // Find unit with given id (and erase it already from units list)
    mutex.lock();
    Unit* unit = NULL;
    for(auto it = units.begin(); it != units.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            units.erase(it);
            unit = *it;
            break;
        }
    }
    mutex.unlock();

    // In case it does not exists, return
    if(unit == NULL) return false;
    
    // Remove unit from (other) relevant lists
    std::string unitType = unit->getType();
    if(unitType.compare("parameter") == 0) removeParameter((UnitParameter*) unit);
    if(unitType.compare("collector") == 0) removeCollector((UnitCollector*) unit);
    if(unitType.compare("audio_output") == 0) removeAudioOutput((UnitAudioOutput*) unit);
    
    // Delete object
    delete unit;
    
    return true;
}

bool Units::rename(std::string oldId, std::string newId) {
    Unit* unit = getUnit(oldId);
    if(unit == NULL) return false; // TODO
    
    Unit* unitNew = getUnit(newId);
    if(unitNew != NULL) return false;
    
    return unit->setId(newId);
}

bool Units::hide(std::string id) {
    Unit* unit = getUnit(id);
    if(unit == NULL) return false; // TODO
    
    unit->hide();
    return true;
}

bool Units::set(std::string id, std::string parameter, std::string value) {
    Unit* unit = getUnit(id);
    if(unit == NULL) return false; // TODO
    
    return unit->setParameter(parameter, value);
}

bool Units::addUnit(Unit* unit) {
    mutex.lock();
    units.push_back(unit);
    mutex.unlock();
    return true;
}

Unit* Units::getUnit(std::string id) {
    mutex.lock();
    Unit* unit = NULL;
    for(auto it = units.begin(); it != units.end(); ++it) {
        if((*it)->getId().compare(id) == 0) {
            unit = *it;
            break;
        }
    }
    mutex.unlock();
    return unit;
}

UnitConstant* Units::createConstant(double value) {
    UnitConstant* constant = new UnitConstant(controller, value);
    mutex.lock();
    constants.push_back(constant);
    mutex.unlock();
    return constant;
}

bool Units::deleteConstant(UnitConstant* constant) {
    mutex.lock();
    bool found = false;
    auto position = std::find(constants.begin(), constants.end(), constant);
    if(position != constants.end()) {
        constants.erase(position);
        delete constant;
        found = true;
    }
    mutex.unlock();
    return found;
}

bool Units::addCollector(UnitCollector* collector) {
    mutex.lock();
    collectors.push_back(collector);
    mutex.unlock();
    return true;
}

bool Units::removeCollector(UnitCollector* collector) {
    bool found = false;
    mutex.lock();
    auto position = std::find(collectors.begin(), collectors.end(), collector);
    if(position != collectors.end()) {
        collectors.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

bool Units::addAudioOutput(UnitAudioOutput* audioOutput) {
    mutex.lock();
    audioOutputs.push_back(audioOutput);
    mutex.unlock();
    return true;
}

bool Units::removeAudioOutput(UnitAudioOutput* output) {
    bool found = false;
    mutex.lock();
    auto position = std::find(audioOutputs.begin(), audioOutputs.end(), output);
    if(position != audioOutputs.end()) {
        audioOutputs.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

bool Units::addParameter(UnitParameter* parameter) {
    mutex.lock();
    bool success = false;
    if(parameters.find(parameter->midiCC) == parameters.end()) {
        parameters[parameter->midiCC] = parameter;
        success = true;
    }
    mutex.unlock();
    return success;
}

UnitParameter* Units::getParameter(int midiCC) {
    mutex.lock();
    UnitParameter* parameter = NULL;
    auto position = parameters.find(midiCC);
    if(position != parameters.end())
        parameter = position->second;
    mutex.unlock();
    return parameter;
}

bool Units::removeParameter(UnitParameter* parameter) {
    bool found = false;
    mutex.lock();
    auto position = parameters.find(parameter->midiCC);
    if(position != parameters.end()) {
        parameters.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

void Units::addKeyEvent(KeyEvent* keyEvent) {
    // Add a copy of this key event to all collectors
    mutex.lock();
    for(auto it = collectors.begin(); it != collectors.end(); ++it) {
        KeyEvent* k = new KeyEvent();
        k->key = keyEvent->key;
        k->stage = keyEvent->stage;
        k->velocity = keyEvent->velocity;
        k->frequency = keyEvent->frequency;
        k->duration = keyEvent->duration;
        k->release = keyEvent->release;
        (*it)->addKeyEvent(k);
    }
    mutex.unlock();
}

void Units::resetUnits() {
    mutex.lock();
    for(auto it = units.begin(); it != units.end(); ++it)
        (*it)->reset();
    mutex.unlock();
}

void Units::resetUnitsKeyDependent() {
    mutex.lock();
    for(auto it = units.begin(); it != units.end(); ++it) {
        Unit* unit = *it;
        if(unit->isKeyDependent())
            unit->reset();
    }
    mutex.unlock();
}

#include <iostream>


bool Units::apply() {
    // Reset buffer
    float* buffer = controller->getAudioDevices()->getBufferOutput();
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    memset(buffer, 0, sizeof(float) * framesPerBuffer * controller->getAudioDevices()->getOutputChannelCount());
    
    // Reset units
    resetUnits();
    
    mutex.lock();

    // Update all collectors
    for(auto it = collectors.begin();it != collectors.end(); ++it) {
        UnitCollector* collector = *it;
        memset(collector->output, 0, sizeof(float) * framesPerBuffer);
        
        // Loop through its list of key events
        MidiState* midiState = controller->getMidiState();
        std::vector<KeyEvent*>* keyEvents = collector->getKeyEvents();
        for(auto it = keyEvents->end() - 1;it != keyEvents->begin() - 1; --it) {
            // Delete the once that have expired
            KeyEvent* keyEvent = *it;
            double releaseTime = collector->getReleaseTime();
            if(keyEvent->release > releaseTime) {
                keyEvents->erase(it);
                delete keyEvent;
                continue;
            }
            
            // Update the output of this event
            midiState->updateKeyEvent(keyEvent);
            mutex.unlock();
            resetUnitsKeyDependent();
            mutex.lock();
            Unit* input = collector->getInput();
            currentKey = keyEvent;
            input->update();
            
            // Add output of unit
            for(int x = 0;x < framesPerBuffer; ++x)
                collector->output[x] += input->output[x];
        }
    }
    
    // Update all units
    for(auto it = units.begin();it != units.end(); ++it) {
        Unit* unit = *it;
        if(!unit->isKeyDependent())
            unit->update();
    }
    
    // Add all outputs to the buffer
    for(auto it = audioOutputs.begin();it != audioOutputs.end(); ++it) {
        UnitAudioOutput* audioOutput = *it;
        int channelCount = controller->getAudioDevices()->getOutputChannelCount();
        for(int channel = 0;channel < channelCount; ++channel) {
            Unit* unit = audioOutput->getChannel(channel);
            for(int x = 0;x < framesPerBuffer; ++x)
                buffer[x * channelCount + channel] += unit->output[x];
        }
    }
    
    mutex.unlock();
    return true;
}
