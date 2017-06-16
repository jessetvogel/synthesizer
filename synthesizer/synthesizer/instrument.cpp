#include <cstring>
#include <cmath>

#include "instrument.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "midistate.hpp"
#include "unit.hpp"
#include "unitconstant.hpp"

Instrument::Instrument(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Initialize arrays
    buffer = new float[controller->getFramesPerBuffer()];
    keyBuffer = new float[controller->getFramesPerBuffer()];
    
    // Default values
    output = new UnitConstant(controller, 0.0);
    keyOutput = new UnitConstant(controller, 0.0);
    keyReleaseTime = 0.0;
}

Instrument::~Instrument() {
    delete[] buffer;
    delete[] keyBuffer;
    
    for(auto it = keyEvents.begin();it != keyEvents.end(); ++it)
        delete *it;
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

void Instrument::update() {
    // Clear the buffer
    memset(keyBuffer, 0, sizeof(float) * controller->getFramesPerBuffer());
    
    // Loop through the list of key events
    for(auto it = keyEvents.end() - 1;it != keyEvents.begin() - 1; --it) {
        KeyEvent* keyEvent = *it;
        if(keyEvent->release > keyReleaseTime) {
            keyEvents.erase(it);
            delete keyEvent;
            continue;
        }
        
        // Update the output of this event
        controller->getMidiState()->updateKeyEvent(keyEvent);
        currentKey = keyEvent;
        controller->resetUnits(true);
        keyOutput->update(this);

        // Add output of keyOutput to the buffer
        for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
            keyBuffer[x] += keyOutput->output[x];
    }
    
    // Now, update the total output of this instrument
    output->update(this);
    
    // Set output, and adjust total output with main volume
    double volume = controller->getMidiState()->mainVolume;
    volume = volume * volume;
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        buffer[x] = output->output[x] * volume;
}

float* Instrument::getBuffer() { return buffer; }

float* Instrument::getKeyBuffer() { return keyBuffer; }
