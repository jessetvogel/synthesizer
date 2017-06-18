#include <algorithm>

#include "controller.hpp"

#include "settings.hpp"

#include "mididevices.hpp"
#include "audiodevices.hpp"

#include "instruments.hpp"
#include "units.hpp"

#include "midistate.hpp"

#include "error.hpp"

Controller::Controller(Settings* settings) {
    // Store pointer to settings
    this->settings = settings;
    
    // Create instances
    midiDevices = new MIDIDevices(this);
    audioDevices = new AudioDevices(this);
    
    instruments = new Instruments(this);
    units = new Units(this);
    
    midiState = new MidiState(this);
    
    // Default values
    active = false;
}

Controller::~Controller() {
    delete midiDevices;
    delete audioDevices;
    
    delete instruments;
    delete units;
    
    delete midiState;
}

bool Controller::start() {
    if(active) return false;
    
    // Create new buffers
    bufferInput = new float[settings->bufferSize];
    bufferOutput = new float[settings->bufferSize];
    
    // Start all devices
    bool success = true;
    success = success && midiDevices->start();
    success = success && audioDevices->start();
    
    active = true;
    return success;
}

bool Controller::stop() {
    if(!active) return false;
    
    // Stop all devices
    bool success = true;
    success = success && midiDevices->stop();
    success = success && audioDevices->stop();
    
    // Delete the buffers
    delete[] bufferInput;
    delete[] bufferOutput;
    
    active = false;
    return success;
}

bool Controller::update() {
    // Update midi devices
    bool success = true;
    success = success && midiDevices->update();
    
    // Apply instruments
    success = success && instruments->apply();
    
    return true;
}
