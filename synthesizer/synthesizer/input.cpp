#include "input.hpp"
#include "log.hpp"
#include <portmidi.h>
#include <iostream>

Input::Input(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Set default values
    active = false;
    inputDevice = -1;
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

void Input::setInputDevice(int n) {
    if(active) {
        Log::warning("Cannot set input device while running");
        return;
    }
    
    inputDevice = n;
}

void Input::start() {
    if(active) {
        Log::warning("Input already started");
        return;
    }
    
    if(inputDevice == -1)
        inputDevice = Pm_GetDefaultInputDeviceID();
    
    const PmDeviceInfo* info = Pm_GetDeviceInfo(inputDevice);
    if(info == NULL) {
        Log::error("Failed to open input device");
        return;
    }
    
    // TODO: remove this
    std::cout << "Opening input device " << info->interf << "," <<  info->name << std::endl;
    
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
}

void Input::update() {
    if(!active) {
        Log::warning("Cannot update if not active");
        return;
    }
    
    PmError result;
    PmEvent event;
    
    while((result = Pm_Poll(inputStream)) > 0) {
        Pm_Read(inputStream, &event, 1);
        PmMessage message = event.message;
        unsigned char status = Pm_MessageStatus(message);
        unsigned char data1 = Pm_MessageData1(message);
        unsigned char data2 = Pm_MessageData2(message);
        controller->getMidiState()->update(status, data1, data2);
    }
}

void Input::stop() {
    if(!active) {
        Log::warning("Tried to stop stream, but it wasn't running");
        return;
    }
    
    Pm_Close(inputStream);
}
