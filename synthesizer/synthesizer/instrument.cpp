#include <cstring>
#include <cmath>

#include "instrument.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "mididevices.hpp"
#include "midistate.hpp"
#include "unit.hpp"
#include "units.hpp"
#include "unitconstant.hpp"

Instrument::Instrument(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Initialize arrays
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    buffer = new float[framesPerBuffer];
    keyBuffer = new float[framesPerBuffer];
    
    // Default values
    output = new UnitConstant(controller, 0.0);
    keyOutput = new UnitConstant(controller, 0.0);
    keyReleaseTime = 0.0;
    
    // By default, active
    active = true;
}

Instrument::~Instrument() {
    delete[] buffer;
    delete[] keyBuffer;
    
    for(auto it = keyEvents.begin();it != keyEvents.end(); ++it)
        delete *it;
}

bool Instrument::setId(std::string id) {
    //    // Only allowed to change id when not yet set
    //    if(this->id.compare(INSTRUMENT_DEFAULT_ID) != 0) {
    //        return false;
    //    }
    this->id = id;
    return true;
}

bool Instrument::setOutput(Unit* unit) {
    // We only allow here for non key dependent units
    if(unit->isKeyDependent()) return false;
    this->output = unit;
    return true;
}

bool Instrument::setKeyOutput(Unit* unit) {
    this->keyOutput = unit;
    return true;
}

bool Instrument::setKeyReleaseTime(double keyReleaseTime) {
    if(keyReleaseTime < 0) return false;
    this->keyReleaseTime = keyReleaseTime;
    return true;
}

Unit* Instrument::getOutput() { return output; }

Unit* Instrument::getKeyOutput() { return keyOutput; }

void Instrument::addKeyEvent(KeyEvent* keyEvent) {
    // Simply add it to the list
    keyEvents.push_back(keyEvent);
}

bool Instrument::update() {
    // Clear the key buffer
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    memset(keyBuffer, 0, sizeof(float) * framesPerBuffer);
    
    // Loop through the list of key events
    MidiState* midiState = controller->getMidiState();
    Units* units = controller->getUnits();
    for(auto it = keyEvents.end() - 1;it != keyEvents.begin() - 1; --it) {
        // Delete the once that have expired
        KeyEvent* keyEvent = *it;
        if(keyEvent->release > keyReleaseTime) {
            keyEvents.erase(it);
            delete keyEvent;
            continue;
        }
        
        // Update the output of this event
        midiState->updateKeyEvent(keyEvent);
        currentKey = keyEvent;
        units->resetUnitsKeyDependent();
        keyOutput->update(this);
        
        // Add output of keyOutput to the buffer
        for(int x = 0;x < framesPerBuffer; ++x)
            keyBuffer[x] += keyOutput->output[x];
    }
    
    // Now, update the total output of this instrument
    output->update(this);
    
    // Set output, and adjust total output with main volume
    double volume = controller->getMidiState()->mainVolume;
    volume = volume * volume; // TODO
    for(int x = 0;x < framesPerBuffer; ++x)
        buffer[x] = output->output[x] * volume;
    
    return true;
}
