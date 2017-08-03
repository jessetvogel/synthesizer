#include <algorithm>

#include "controller.hpp"
#include "settings.hpp"
#include "mididevices.hpp"
#include "audiodevices.hpp"
#include "nodes.hpp"
#include "monitor.hpp"
#include "keystate.hpp"
#include "status.hpp"

Controller::Controller(Settings* settings) {
    // Store pointer to settings
    this->settings = settings;
    
    // Create instances
    midiDevices = new MIDIDevices(this);
    audioDevices = new AudioDevices(this);
    nodes = new Nodes(this);
    monitor = new Monitor(this);
    keyState = new KeyState(this);
        
    // Default values
    active = false;
}

Controller::~Controller() {
    delete midiDevices;
    delete audioDevices;
    delete nodes;
    delete monitor;
    delete keyState;
}

bool Controller::start() {
    if(active) return true;
    
    mutex.lock();
    
    // Start all devices
    bool success = true;
    success = success && midiDevices->start();
    success = success && audioDevices->start();
    
    mutex.unlock();
    
    active = true;
    return success;
}

bool Controller::stop() {
    if(!active) return true;
    
    mutex.lock();
    
    // Stop all devices
    bool success = true;
    success = success && midiDevices->stop();
    success = success && audioDevices->stop();
    
    mutex.unlock();
    
    active = false;
    return success;
}

bool Controller::clear() {
    // Need to be stopped to clear
    bool wasActive = active;
    if(active) stop();

    bool success = nodes->clear();
    
    if(wasActive) start();
    return success;
}

bool Controller::update() {
    // If active was set to false, just stop, no errors need to be given
    if(!active) return false;
    
    bool success = true;
    mutex.lock();

    // Update midi devices
    success = success && midiDevices->update();
    
    // Apply nodes
    success = success && nodes->apply();
    
    mutex.unlock();
    return success;
}

bool Controller::play(std::string state) {
    Status::addExtra("state");
    
    if(state.compare("start") == 0)     return start();
    if(state.compare("stop") == 0)      return stop();
    if(state.compare("toggle") == 0)    return active ? stop() : start();
    
    Status::addError("Invalid argument provided");
    return false;
}
