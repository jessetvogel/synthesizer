#include "mididevice.hpp"
#include "controller.hpp"
#include "midistate.hpp"

#include "status.hpp"

int MIDIDevice::DEVICE_ID_DEFAULT = -1;

MIDIDevice::MIDIDevice(Controller* controller, int deviceId) {
    // Store info
    this->controller = controller;
    
    // Device id -1 indicates default device id
    this->deviceId = (deviceId == DEVICE_ID_DEFAULT) ? Pm_GetDefaultInputDeviceID() : deviceId;
    
    // By default, not active
    active = false;
}

bool MIDIDevice::start() {
    // Make sure device did not already start
    if(active) return true;
    
    const PmDeviceInfo* info = Pm_GetDeviceInfo(deviceId);
    if(info == NULL) { Status::addError("MIDI device does not exist"); return false; }
    
    PmError err = Pm_OpenInput(&inputStream,
                               deviceId,
                               NULL, // driver info
                               0, // use default input size
                               NULL,
                               (void*) NULL); // void * time_info
    
    if(err != pmNoError) { Status::addError("Failed to open MIDI device"); return false; }
    
    /*
     Pm_SetFilter(midi_in, PM_FILT_REALTIME | PM_FILT_AFTERTOUCH |
     PM_FILT_PROGRAM | PM_FILT_PITCHBEND | PM_FILT_CONTROL);
     */
    
    Pm_SetFilter(inputStream, PM_FILT_REALTIME); // TODO: what are these filters?
    
    active = true;
    return true;
}

bool MIDIDevice::stop() {
    // Try to close the stream
    bool success = true;
    if(Pm_Close(inputStream) != pmNoError) {
        Status::addError("Failed to close MIDI device");
        success = false;
    }
    
    active = false;
    return success;
}

bool MIDIDevice::update() {
    // Make sure device started
    if(!active) { Status::addError("MIDI device not yet started"); return false; }
    
    // Read all incoming messages
    PmError result;
    PmEvent event;
    
    while((result = Pm_Poll(inputStream)) > 0) {
        Pm_Read(inputStream, &event, 1);
        PmMessage message = event.message;
        unsigned char status = Pm_MessageStatus(message);
        unsigned char data1 = Pm_MessageData1(message);
        unsigned char data2 = Pm_MessageData2(message);
        controller->getMidiState()->addEvent(status, data1, data2);
    }
    
    if(result != pmNoError) { Status::addError("Error in reading from MIDI device"); return false; }
    
    controller->getMidiState()->update();
    return true;
}
