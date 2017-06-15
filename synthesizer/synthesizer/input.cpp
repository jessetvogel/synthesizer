#include <iostream>
#include <portmidi.h>

#include "input.hpp"
#include "controller.hpp"
#include "midistate.hpp"

#include "error.hpp"

Input::Input(Controller* controller, int inputDevice) {
    // Store pointer to controller object
    this->controller = controller;
    this->inputDevice = inputDevice;
    
    // Set default values
    active = false;
}

bool Input::start() {
    if(active) {
        Error::addError(Error::INPUT_ALREADY_STARTED);
        return false;
    }
    
    if(inputDevice == -1)
        inputDevice = Pm_GetDefaultInputDeviceID();
    
    const PmDeviceInfo* info = Pm_GetDeviceInfo(inputDevice);
    if(info == NULL) {
        Error::addError(Error::INPUT_DEVICE_NOT_EXISTS);
        return false;
    }
    
    PmError err = Pm_OpenInput(&inputStream,
                 inputDevice,
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

bool Input::update() {
    if(!active) {
        Error::addError(Error::INPUT_NOT_YET_STARTED);
        return false;
    }
    
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

bool Input::stop() {
    if(!active) {
        Error::addError(Error::INPUT_NOT_YET_STARTED);
        return false;
    }
    
    active = false;
    if(Pm_Close(inputStream) != pmNoError) {
        Error::addError(Error::INPUT_CANNOT_CLOSE_INPUT);
        return false;
    }
    return true;
}

int Input::getInputDevice() {
    return inputDevice;
}

int Input::amountOfDevices() {
    return Pm_CountDevices();
}

const char* Input::deviceName(int n) {
    const PmDeviceInfo* info = Pm_GetDeviceInfo(n);
    if(info == NULL) {
        Error::addError(Error::INPUT_DEVICE_NOT_EXISTS);
        return NULL;
    }
    
    return Pm_GetDeviceInfo(n)->name;
}

bool Input::isInput(int n) {
    const PmDeviceInfo* info = Pm_GetDeviceInfo(n);
    if(info == NULL) return false;
    return Pm_GetDeviceInfo(n)->input > 0;
}
