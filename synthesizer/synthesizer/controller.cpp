#include <algorithm>

#include "controller.hpp"

#include "settings.hpp"
#include "options.hpp"

#include "mididevices.hpp"
#include "audiodevices.hpp"

#include "units.hpp"
#include "blocks.hpp"

#include "midistate.hpp"

#include "status.hpp"

Controller::Controller(Settings* settings) {
    // Store pointer to settings
    this->settings = settings;
    
    // Create instances
    options = new Options(this);
    
    midiDevices = new MIDIDevices(this);
    audioDevices = new AudioDevices(this);
    
    units = new Units(this);
    blocks = new Blocks(this);
    
    midiState = new MidiState(this);
        
    // Default values
    active = false;
}

Controller::~Controller() {
    delete midiDevices;
    delete audioDevices;
    
    delete units;
    delete blocks;
    
    delete midiState;
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

bool Controller::reset() {
    // TODO
    return true;
}

bool Controller::update() {
    // If active was set to false, just stop, no errors need to be given
    if(!active) return false;
    
    // Update midi devices
    bool success = true;
    success = success && midiDevices->update();
    
    // Apply units
    success = success && units->apply();
    
    return true;
}

bool Controller::play(std::string state) {
    Status::addExtra("state");
    
    if(state.compare("start") == 0)     return start();
    if(state.compare("stop") == 0)      return stop();
    if(state.compare("toggle") == 0)    return active ? stop() : start();
    
    Status::addError("unknown state. groetjes van controller");
    return false;
}
