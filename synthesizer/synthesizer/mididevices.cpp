#include <portmidi.h>

#include "controller.hpp"
#include "mididevices.hpp"
#include "mididevice.hpp"

#include "status.hpp"

MIDIDevices::MIDIDevices(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // By default, not active
    active = false;
}

MIDIDevices::~MIDIDevices() {
    mutex.lock();
    for(auto it = devices.begin(); it != devices.end(); ++it)
        delete (*it);
    mutex.unlock();
}

bool MIDIDevices::start() {
    // Start all devices
    bool success = true;
    mutex.lock();
    for(auto it = devices.begin(); it != devices.end(); ++it) {
        MIDIDevice* device = *it;
        success = success && device->start();
    }
    active = true;
    mutex.unlock();
    return success;
}

bool MIDIDevices::stop() {
    // Stop all devices
    bool success = true;
    mutex.lock();
    for(auto it = devices.begin(); it != devices.end(); ++it) {
        MIDIDevice* device = *it;
        success = success && device->stop();
    }
    mutex.unlock();
    active = false;
    return success;
}

bool MIDIDevices::update() {
    // Update all devices
    bool success = true;
    mutex.lock();
    for(auto it = devices.begin(); it != devices.end(); ++it) {
        MIDIDevice* device = *it;
        success = success && device->update();
    }
    mutex.unlock();
    return success;
}

bool MIDIDevices::add(int n) {
    // Make sure the given id is actually an midi input device
    if(!isInput(n)) { Status::addError("Provided MIDI device does not exist"); return false; }
    
    // Make sure it isn't added already
    if(get(n) != NULL) return true;
    
    // Add new device to the list
    mutex.lock();
    MIDIDevice* device = new MIDIDevice(controller, n);
    if(active) device->start();
    devices.push_back(device);
    mutex.unlock();
    return true;
}

MIDIDevice* MIDIDevices::get(int n) {
    MIDIDevice* midiDevice = NULL;
    mutex.lock();
    for(auto it = devices.begin(); it != devices.end(); ++it) {
        if((*it)->getDeviceId() == n) {
            midiDevice = *it;
            break;
        }
    }
    mutex.unlock();
    return midiDevice;
}

bool MIDIDevices::remove(int n) {
    // Remove device with the given id from the list
    mutex.lock();
    bool found = false;
    for(auto it = devices.begin(); it != devices.end(); ++it) {
        MIDIDevice* device = *it;
        if(device->getDeviceId() == n) {
            if(device->isActive()) device->stop();
            devices.erase(it);
            delete device;
            found = true;
            break;
        }
    }
    mutex.unlock();
    return found;
}

int MIDIDevices::amountOfDevices() {
    return Pm_CountDevices();
}

const char* MIDIDevices::deviceName(int n) {
    const PmDeviceInfo* info = Pm_GetDeviceInfo(n);
    if(info == NULL) { Status::addError("Provided MIDI device does not exist"); return NULL; }
    return Pm_GetDeviceInfo(n)->name;
}

bool MIDIDevices::isInput(int n) {
    const PmDeviceInfo* info = Pm_GetDeviceInfo(n);
    if(info == NULL) return false;
    return Pm_GetDeviceInfo(n)->input > 0;
}
