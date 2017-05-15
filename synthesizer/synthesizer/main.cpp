#include <iostream>
#include <portaudio.h>
#include <portmidi.h>
#include "controller.hpp"
#include "log.hpp"

int main(int argc, char *argv[]) {
    // Initialize stuff (TODO: put this in some audio class or something, and check for errors)
    Pm_Initialize();
    Pa_Initialize();
    
    
    // Setup
    Controller* controller = new Controller();
    
    Log::output("Input devices");
    controller->listInputDevices();
    Log::output("\nOutput devices");
    controller->listOutputDevices();
    controller->setInputDevice(1);
    controller->setOutputDevice(1);
    Log::output("");
    
    controller->start();
    
    Log::output("Press enter to stop");
    getchar();
    
    controller->stop();
    
    // Cleanup stuff
    delete controller;

    Pm_Terminate();
    Pa_Terminate();
    
    return 0;
}
