#include <iostream>
#include <portaudio.h>
#include <portmidi.h>

#include "controller.hpp"
#include "parser.hpp"
#include "sample.hpp"

#include "log.hpp"

int main(int argc, char *argv[]) {
    // Initialize stuff (TODO: put this in some audio class or something, and check for errors)
    Pm_Initialize();
    Pa_Initialize();
    Sample::initialize();
    
    // Setup
    Controller* controller = new Controller();
    
    Log::output("Input devices");
    controller->listInputDevices();
    Log::output("\nOutput devices");
    controller->listOutputDevices();
    controller->setInputDevice(1);
    controller->setOutputDevice(1);
    Log::output("");
    
    Parser parser(controller, "/Users/jessetvogel/Desktop/synthesizer.txt");

    controller->start();
    
    Log::output("Press enter to stop");
    getchar();
    
    controller->stop();
    
    // Cleanup stuff
    delete controller;
    
    // TODO: cleanup samples and envelopes
    
    Pm_Terminate();
    Pa_Terminate();
    
    return 0;
}
