#include <iostream>
#include <portmidi.h>

#include "input.hpp"
#include "controller.hpp"
#include "midistate.hpp"

#include "log.hpp"

Input::Input(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Set default values
    active = false;
}

int Input::amountOfDevices() {
    return Pm_CountDevices();
}

const char* Input::deviceName(int n) {
    // TODO: maybe check if it is null
    return Pm_GetDeviceInfo(n)->name;
}

bool Input::isInput(int n) {
    return Pm_GetDeviceInfo(n)->input > 0;
}

bool Input::start() {
    if(active) return false;
    
    int inputDevice = controller->getInputDevice();
    
    if(inputDevice == -1)
        inputDevice = Pm_GetDefaultInputDeviceID();
    
    const PmDeviceInfo* info = Pm_GetDeviceInfo(inputDevice);
    if(info == NULL) return false;
    
    Pm_OpenInput(&inputStream,
                 inputDevice,
                 NULL, // driver info
                 0, // use default input size
                 NULL,
                 (void*) NULL); // void * time_info
    
    /*
     Pm_SetFilter(midi_in, PM_FILT_REALTIME | PM_FILT_AFTERTOUCH |
     PM_FILT_PROGRAM | PM_FILT_PITCHBEND | PM_FILT_CONTROL);
     */
    
    Pm_SetFilter(inputStream, PM_FILT_REALTIME);
    
    active = true;
    return true;
}

bool Input::update() {
    if(!active) return false;
    
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
    
    // TODO: check for errors?
    
    controller->getMidiState()->update();
    return true;
}

bool Input::stop() {
    if(!active) return false;
    
    active = false;
    Pm_Close(inputStream);
    return true;
}
