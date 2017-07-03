#include "instruments.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "units.hpp"
#include "settings.hpp"
#include "keyevent.hpp"

Instruments::Instruments(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
}

Instruments::~Instruments() {
    mutex.lock();
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        delete *it;
    }
    mutex.unlock();
}

Instrument* Instruments::get(std::string id) {
    mutex.lock();
    Instrument* instrument = NULL;
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        if((*it)->getId().compare(id) == 0) {
            instrument = *it;
            break;
        }
    }
    mutex.unlock();
    return instrument;
}

bool Instruments::create(std::string id) {
    mutex.lock();
    // TODO: check if instrument with 'id' already exists
    Instrument* instrument = new Instrument(controller);
    instrument->setId(id);
    instruments.push_back(instrument);
    mutex.unlock();
    return true;
}

bool Instruments::remove(std::string id) {
    mutex.lock();
    bool found = false;
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        if((*it)->getId().compare(id) == 0) {
            instruments.erase(it);
            delete *it;
            found = true;
            break;
        }
    }
    mutex.unlock();
    return found;
}

bool Instruments::setActive(std::string id, bool active) {
    Instrument* instrument = get(id);
    if(instrument == NULL) return false; // TODO: errorzzz..
    
    instrument->active = active;
    return true;
}

bool Instruments::setOutput(std::string id, std::string output) {
    Instrument* instrument = get(id);
    if(instrument == NULL) return false; // TODO: errorzz.
    
    Unit* unit = controller->getUnits()->get(output);
    if(unit == NULL) return false;
    
    return instrument->setOutput(unit);
}

bool Instruments::setKeyOutput(std::string id, std::string keyOutput) {
    Instrument* instrument = get(id);
    if(instrument == NULL) return false; // TODO: errorzz.
    
    Unit* unit = controller->getUnits()->get(keyOutput);
    if(unit == NULL) return false;
    
    return instrument->setKeyOutput(unit);
}

bool Instruments::setKeyReleaseTime(std::string id, double keyReleaseTime) {
    Instrument* instrument = get(id);
    if(instrument == NULL) return false; // TODO: errorzz.
    
    instrument->keyReleaseTime = keyReleaseTime;
    return true;
}

bool Instruments::apply() {
    // Clear buffer
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    float* buffer = controller->getBufferOutput();
    memset(buffer, 0, sizeof(float) * framesPerBuffer);
    
    // Reset all units
    controller->getUnits()->resetUnits();
    
    // Update all instruments, and add their result to the buffer
    bool success = true;
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        if(!((*it)->active)) continue;
        success = success && (*it)->update();
        float* instrumentBuffer = (*it)->getBuffer();
        for(int x = 0;x < framesPerBuffer; ++x)
            buffer[x] += instrumentBuffer[x];
    }
    return success;
}

void Instruments::addKeyEvent(KeyEvent* keyEvent) {
    // Add a copy of this key event to all instruments
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        KeyEvent* k = new KeyEvent();
        k->key = keyEvent->key;
        k->stage = keyEvent->stage;
        k->velocity = keyEvent->velocity;
        k->frequency = keyEvent->frequency;
        k->duration = keyEvent->duration;
        k->release = keyEvent->release;
        (*it)->addKeyEvent(k);
    }
}

