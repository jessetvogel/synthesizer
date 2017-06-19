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

bool Instruments::add(Instrument* instrument) {
    mutex.lock();
    instruments.push_back(instrument);
    mutex.unlock();
    return true;
}

bool Instruments::remove(Instrument* instrument) {
    mutex.lock();
    bool found = false;
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        if((*it) == instrument) {
            instruments.erase(it);
            delete instrument;
            found = true;
            break;
        }
    }
    mutex.unlock();
    return found;
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

