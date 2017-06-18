#include "mididevice.hpp"
#include "controller.hpp"
#include "midistate.hpp"

#include "error.hpp"

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
    if(active) {
        Error::addError(Error::INPUT_ALREADY_STARTED);
        return false;
    }
    
    const PmDeviceInfo* info = Pm_GetDeviceInfo(deviceId);
    if(info == NULL) {
        Error::addError(Error::INPUT_DEVICE_NOT_EXISTS);
        return false;
    }
    
    PmError err = Pm_OpenInput(&inputStream,
                               deviceId,
                               NULL, // driver info
                               0, // use default input size
                               NULL,
                               (void*) NULL); // void * time_info
    
    if(err != pmNoError) {
        Error::addError(Error::INPUT_CANNOT_OPEN_INPUT);
        return false;
    }
    
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
        Error::addError(Error::INPUT_CANNOT_CLOSE_INPUT);
        success = false;
    }
    
    active = false;
    return success;
}

bool MIDIDevice::update() {
    // Make sure device started
    if(!active) {
        Error::addError(Error::INPUT_NOT_YET_STARTED);
        return false;
    }
    
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
    
    if(result != pmNoError) {
        Error::addError(Error::INPUT_READING);
        return false;
    }
    
    controller->getMidiState()->update();
    return true;
}
