#include <algorithm>

#include "controller.hpp"

#include "settings.hpp"
#include "options.hpp"

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
    options = new Options(this);
    
    midiDevices = new MIDIDevices(this);
    audioDevices = new AudioDevices(this);
    
    instruments = new Instruments(this);
    units = new Units(this);
    
    midiState = new MidiState(this);
    
    // Create new buffers
    bufferInput = new float[settings->bufferSize];
    bufferOutput = new float[settings->bufferSize];
    memset(bufferInput, 0, sizeof(float) * settings->bufferSize);
    memset(bufferOutput, 0, sizeof(float) * settings->bufferSize);
    
    // Default values
    active = false;
}

Controller::~Controller() {
    delete midiDevices;
    delete audioDevices;
    
    delete instruments;
    delete units;
    
    delete midiState;
    
    delete[] bufferInput;
    delete[] bufferOutput;
}

bool Controller::start() {
    if(active) return false;
    
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
    
    active = false;
    return success;
}

bool Controller::update() {
    // If active was set to false, just stop, no errors need to be given
    if(!active) return false;
    
    // Update midi devices
    bool success = true;
    success = success && midiDevices->update();
    
    // Apply instruments
    success = success && instruments->apply();
    
    return true;
}
