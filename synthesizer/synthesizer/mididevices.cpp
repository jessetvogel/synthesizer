#include <portmidi.h>
#include <fstream>

#include "controller.hpp"
#include "settings.hpp"
#include "mididevices.hpp"
#include "mididevice.hpp"
#include "commands.hpp"

#include "status.hpp"

MIDIDevices::MIDIDevices(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // By default, not active
    active = false;
    
    // Load
    load();
}

MIDIDevices::~MIDIDevices() {
    mutex.lock();
    for(auto it = devices.begin(); it != devices.end(); ++it)
        delete (*it);
    mutex.unlock();
}

bool MIDIDevices::load() {
    std::string settingsDirectory = controller->getSettings()->directory;
    if(settingsDirectory.length() == 0) return true;
    
    std::ifstream input(settingsDirectory + DIRECTORY_SEPARATOR + "midi_devices");
    if(input.fail()) { Status::addError("Failed to open midi devices file to load data"); return false; }
    
    // Add all listed devices (if available) and add them to the list of preferered devices
    std::string line;
    int n = amountOfDevices();
    while(std::getline(input, line)) {
        for(int i = 0;i < n; ++i) {
            if(line.compare(deviceName(i)) == 0)
                add(i);
        }
        preferredDevices.push_back(line);
    }
    input.close();
    return true;
}

bool MIDIDevices::store() {
    std::string settingsDirectory = controller->getSettings()->directory;
    if(settingsDirectory.length() == 0) return true;

    std::ofstream output(settingsDirectory + DIRECTORY_SEPARATOR + "midi_devices");
    if(output.fail()) { Status::addError("Failed to open midi devices file to store data"); return false; }
    
    // Output all preferred devices
    for(auto it = preferredDevices.begin();it != preferredDevices.end(); ++it)
        output << *it << std::endl;

    output.close();
    return true;
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
    
    // If not yet in the preferred list, put it
    std::string name = deviceName(n);
    if(std::find(preferredDevices.begin(), preferredDevices.end(), name) == preferredDevices.end())
        preferredDevices.push_back(name);
    
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
    
    // If in the list of preferred devices, remove it
    preferredDevices.erase(std::remove(preferredDevices.begin(), preferredDevices.end(), deviceName(n)), preferredDevices.end());
    
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
